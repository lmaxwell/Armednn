## Introduction

modular neural network inference engine

### features
. warmup
. node memory share 
. operator registry
. cross-platform 


## INSTALL

```
git submodule init 
git submodule update
```

### compile Eigen without MKL 
```
make 
```

### compile Eigen with MKL 
```
make MKLROOT=MKL/path/in/your/PC 
```
MKLROOT/include and MKLROOT/lib/intel64_lin should exist

## 1. Usage

### 1.1 simple dense 

```c++
    int L=2000;
    int C=256;

    auto input_node=make_input("input");


    ConfigMap config;
    config.insert({"activation",{(std::string)"tanh"}});
    config.insert({"dim0",{(uint32_t)C}});
    config.insert({"dim1",{(uint32_t)C}});
    ParamMap param;
    param.insert({"weight",{Matrix::Identity(C,C)}});
    param.insert({"bias",{Matrix::Ones(1,C)}});

    Arm arm(config,param);

    auto dense_0=make_node("Dense",arm,input_node->output(),"dense-0");
    auto dense_1=make_node("Dense",arm,dense_0->output(),"dense-1");
    auto dense_2=make_node("Dense",arm,dense_1->output(),"dense-2");
    auto dense_3=make_node("Dense",arm,dense_2->output(),"dense-3");


    Matrix temp=Matrix::Identity(L,C);
    input_node->feed(temp);

    dense_0->run();
    dense_1->run();
    dense_2->run();
    dense_3->run();
```



### 1.2  stateful 

```c++
    int L=2000;
    int C=256;

    auto input_node=make_input("input");


    ConfigMap config;
    config.insert({"activation",{(std::string)"tanh"}});
    config.insert({"dim0",{(uint32_t)C}});
    config.insert({"dim1",{(uint32_t)C}});
    ParamMap param;
    param.insert({"weight",{Matrix::Identity(C,C)}});
    param.insert({"bias",{Matrix::Ones(1,C)}});

    Arm arm(config,param);

    auto dense_0=make_node("Dense",arm,input_node->output(),"dense-0");
   
    
    ConfigMap config_split_0;
    config_split_0.insert({"num_split",{(uint32_t)2}});
    Arm arm_split_0(config_split_0);
  
    auto split_0=make_node("Split",arm_split_0,dense_0->output(),"split-0");


    
    ConfigMap config_fpool_0;
    config_fpool_0.insert({"output_channels",{(uint32_t)(C/2)}});
    Arm arm_fpool_0(config_fpool_0);
    auto init_state_node=make_state("init_state",Matrix::Zero(1,C/2));
    auto fpool_0=make_node("Fpooling",arm_fpool_0, {split_0->output(0),split_0->output(1),init_state_node->output(0)},"fpool-0");
    
    Matrix temp=Matrix::Identity(L,C);
    input_node->feed(temp);

    dense_0->run();
    split_0->run();
    init_state_node->run(); 
    fpool_0->run();
    fpool_0->run();
    fpool_0->run();
    init_state_node->run(); // reset state
    fpool_0->run();
```
### 1.3 share node memory
```c++
    int L=2000;
    int C=256;

    auto input_node=make_input("input");


    ConfigMap config;
    config.insert({"activation",{(std::string)"tanh"}});
    config.insert({"dim0",{(uint32_t)C}});
    config.insert({"dim1",{(uint32_t)C}});
    ParamMap param;
    param.insert({"weight",{Matrix::Identity(C,C)}});
    param.insert({"bias",{Matrix::Ones(1,C)}});

    Arm arm(config,param);

    auto dense_0=make_node("Dense",arm,input_node->output(),"dense-0");
    auto dense_1=make_node("Dense",arm,dense_0->output(),"dense-1");
    auto dense_2=make_node("Dense",arm,dense_1->output(),"dense-2",dense_0->output()); //share memory with dense_0
    auto dense_3=make_node("Dense",arm,dense_2->output(),"dense-3",dense_1->output()); //share memory with dense_1 
    
    /*
    can also be
    auto dense_0=make_node("Dense",arm,input_node->output(),"dense-0");
    auto dense_1=make_node("Dense",arm,dense_0->output(),"dense-1",input_node->output()); //share
    auto dense_2=make_node("Dense",arm,dense_1->output(),"dense-2",dense_0->output()); //share 
    auto dense_3=make_node("Dense",arm,dense_2->output(),"dense-3",input_node->output()); //share 
    */

    Matrix temp=Matrix::Identity(L,C);
    input_node->feed(temp);

    dense_0->run();
    dense_1->run();
    dense_2->run();
    dense_3->run();

    INFO<<dense_3->output(0)->get();

```

### 1.4 share node memory by SharedData
```c++
    std::unique_ptr<Node> make_dense(float* weights, std::string name, 
        uint32_t input_channels, uint32_t output_channels, std::string activation,
        DataPtr inputs, DataPtr outputs = DataPtr())
    {
        ConfigMap config;
        config.insert({"dim0", {input_channels}});
        config.insert({"dim1", {output_channels}});
        config.insert({"activation", {activation}});

        auto weight = Eigen::Map<Matrix>(weights, input_channels, output_channels);
        auto offset = input_channels * output_channels;
        auto bias = Eigen::Map<Matrix>(weights + offset, 1, output_channels);

        ParamMap param;
        param.insert({"weight", {weight}});
        param.insert({"bias",{Matrix::Ones(1,C)}});
        Arm arm(config, param);

        return make_node("Dense", arm, inputs, name, outputs);
    }

    int L=2000;
    int C=256;

    auto input_node=make_input("input");

    auto weight = Matrix::Ones(C + 1, C);
    SharedData shared;

    auto dense_0 = make_dense(weight.data(), "dense-0", C, C, "tanh", input_node->output());
    auto dense_1 = make_dense(weight.data(), "dense-1", C, C, "tanh", dense_0->output());
    shared.put(dense_1->input());
    auto dense_2 = make_dense(weight.data(), "dense-2", C, C, "tanh", dense_1->output(), shared.get(1));//share
    shared.put(dense_2->input());
    auto dense_3 = make_dense(weight.data(), "dense-3", C, C, "tanh", dense_2->output(), shared.get(1));//share
    
    /*
    can also be
    auto dense_0 = make_dense(weight.data(), "dense-0", C, C, "tanh", input_node->output());
    shared.put(dense_0->input());
    auto dense_1 = make_dense(weight.data(), "dense-1", C, C, "tanh", dense_0->output(), shared.get(1));//share
    shared.put(dense_1->input());
    auto dense_2 = make_dense(weight.data(), "dense-2", C, C, "tanh", dense_1->output(), shared.get(1));//share
    shared.put(dense_2->input());
    auto dense_3 = make_dense(weight.data(), "dense-3", C, C, "tanh", dense_2->output(), shared.get(1));//share
    */

    Matrix temp=Matrix::Identity(L,C);
    input_node->feed(temp);

    dense_0->run();
    dense_1->run();
    dense_2->run();
    dense_3->run();

    INFO<<dense_3->output(0)->get();

```



## 2. Data 

```c++
Data d; 

d.allocate(10,10); // allocate 10x10

d.allocate(5,5); // no allocate here;

d.allocate(100,1000); // re allocate

audo mat=d.get() // return a Eigen::Map<Matrix>, not own any data, is a Mapping of "Real Matrix" in Data

Matrix a(100,100);

d.get()=a; // assign, 
mat=a; // the same 


// make sure enough size before assign
Matrix b(1000,1000);
d.allocate(1000,1000); // mat is useless now
auto mat2=d.get(); // call get() again after every allocate()
mat2=b;


```

*  remember to allocate() before doing any assign
*  if use local reference, call get() again after every allocate()  

### 

## 3. operation registry
```C++
REGISTER_OP(Dense).add_config<uint32_t>("dim0","dimension 0")
                     .add_config<uint32_t>("dim1","dimension 1")
                     .add_config<std::string>("activation","activation function type")
                     .add_param("weight",{"dim0","dim1"})
                     .add_param("bias",{"1","dim1"})
                     .set_num_input("1")
                     .set_num_output("1");
```
### config
```C++
add_config<T>(std::string name, std::string desc)
```
* T:type

* name: config name

* desc: config description

### param
```C++
add_param(std::string name, std::vector<std::string> shape_mapping)
add_param(std::string name, std::function<std::vector<uint32_t>(ConfigMap&)> shape_func)
```
* name: parameter name

* shape_mapping: shape mapping to config names

* shape_func:   a function to get shape from Config


### num_input & num_output
```c++
REGISTER_OP(Split).add_config<uint32_t>("num_split","number of split outputs, along last dim")
                     .set_num_input("1")
                     .set_num_output("num_split");
```
```c++
set_num_input(std::string num_input)
```
* num_input: if exists in config, get mapping number from config; else get by (unit32_t)num_input
* num_output: ...





