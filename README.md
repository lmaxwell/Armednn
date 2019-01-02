
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


std::unique_ptr<Node> make_node(DataPtr&& inputs, std::unique_ptr<Armed> armed)

```


### 

## 2. operation registry
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
```
* name: parameter name

* shape_mapping: shape mapping to config names
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
```
* name: state name
* shape_mappint: shape_mapping to config names


### num_input & num_output
```c++
REGISTER_OP(Split).add_config<uint32_t>("num_split","number of split outputs, along last dim")
                     .set_num_input("1")
                     .set_num_output("num_split");
```
```c++
set_num_input(std::string num_input)
```
num_input: if exists in config, get mapping number from config; else get by (unit32_t)num_input
num_output: ...





