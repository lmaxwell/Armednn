
# define a inference network 


```c++
class MyNet:public Network
{
    public:
        void build()
        {
            auto xx= Input();


            auto x=Layer::create("Dense",
                                 {{"activation","tanh"},{"dim0","10"},{"dim1","10"}},
                                 "Dense-0")(xx);

            auto y=Layer::create("Split",{{"num_split","2"}},"Split-0")(x);

            auto z=Layer::create("Dense",
                            {{"activation","tanh"},{"dim0","5"},{"dim1","5"}},
                            "Dense-1")({y[0]});

            Output(z);
        }

}
```
