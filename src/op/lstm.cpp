#include "./lstm.h"
namespace Armednn
{


void lstm_step(Eigen::Ref<Matrix> XW,Eigen::Ref<Matrix> HR,
        Eigen::Ref<Matrix> cell_state,Eigen::Ref<Matrix> hidden_state)
{
    XW=HR+XW;
    auto channels=XW.cols()/4;

    auto input_gate=XW.block(0,0,1,channels);
    activation("sigmoid")(input_gate);
    auto cell=XW.block(0,channels,1,channels);
    activation("tanh")(cell);
    auto forget_gate=XW.block(0,channels*2,1,channels);
    activation("sigmoid")(forget_gate);
    auto output_gate=XW.block(0,channels*3,1,channels);
    activation("sigmoid")(output_gate);

    cell_state.array() = forget_gate.array()*cell_state.array();
    cell_state.array() += input_gate.array()*cell.array();

    hidden_state=cell_state.array().tanh();
    hidden_state.array()=output_gate.array()*hidden_state.array();

}


void LSTM::inference(DataPtr& inputs, DataPtr& outputs, Arm& arm)
{
    auto output_channels=arm.config<uint32_t>("output_channels");
    auto input=inputs[0]->get();
    auto cell_state=inputs[1]->get();
    auto hidden_state=inputs[2]->get();

    outputs[0]->allocate(input.rows(),output_channels);
    auto output=outputs[0]->get();
    
    auto bidirection=arm.config<bool>("bidirection");
    auto weight_output_channels = bidirection?output_channels/2:output_channels;
   
    XW.allocate(input.rows(),output_channels*4);
    XW.get().noalias() = input * arm.param("weightW");
    XW.get() += arm.param("bias").replicate(input.rows(),1);

    HR.allocate(1,weight_output_channels*4);
    if(bidirection)
    {
        for(uint32_t i=0;i<input.rows();i++)
        {
            HR.get().noalias() = hidden_state.leftCols(weight_output_channels) * 
                arm.param("weightR").topRows(weight_output_channels) ;

            lstm_step(XW.get().row(i).leftCols(4*weight_output_channels), 
                    HR.get(), 
                    cell_state.leftCols(weight_output_channels),
                    hidden_state.leftCols(weight_output_channels));
            output.row(i).leftCols(weight_output_channels)=hidden_state.leftCols(weight_output_channels);

            HR.get().noalias() = hidden_state.rightCols(weight_output_channels) * 
                arm.param("weightR").bottomRows(weight_output_channels) ;

            lstm_step(XW.get().row(input.rows()-1-i).rightCols(4*weight_output_channels), 
                    HR.get(), 
                    cell_state.rightCols(weight_output_channels),
                    hidden_state.rightCols(weight_output_channels));
            
            output.row(input.rows()-1-i).rightCols(weight_output_channels)=hidden_state.rightCols(weight_output_channels);

        }
    }
    else
    {
        for(uint32_t i=0;i<input.rows();i++)
        {
            HR.get().noalias() = hidden_state * arm.param("weightR");
            lstm_step(XW.get().row(i), HR.get(), cell_state, hidden_state);
            
            output.row(i)=hidden_state;

        }
    }
}




}//Armednn
