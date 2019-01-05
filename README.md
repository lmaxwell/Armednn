
## 1. Usage

### 1.1 definition
 
* arm = { config , param } 
 
* arm + op =armed; // check arm and op compatible

* armed + inputs = node; // node { armed , inputs, outputs }, outputs is auto allocated 

### 1.2  interface

```c++
std::unique_ptr<Operator> make_op(std::string type,std::string name)


std::unique_ptr<Armed> make_armed(Arm& arm, std::unique_ptr<Operator> op)


std::unique_ptr<Node> make_node(DataPtr& inputs, std::unique_ptr<Armed> armed)

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


// make sure enough size before assign
Matrix b(1000,1000);
d.allocate(1000,1000);
d.get()=b;

mat=d.get(); // call get() again after every allocate()

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
### state

for stateful operation
```C++
REGISTER_OP(Fpooling).add_config<uint32_t>("output_channels","output channels")
                     .add_state("h",{"1","output_channels"})
                     .set_num_input("2")
                     .set_num_output("1");
```
```c++
add_state(std::string name,std::vector<std::string> shape_mapping)
add_state(std::string name,std::function<std::vector<uint32_t>(ConfigMap&)> shape_func)
```
* name: state name
* shape_mapping: shape_mapping to config names
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





