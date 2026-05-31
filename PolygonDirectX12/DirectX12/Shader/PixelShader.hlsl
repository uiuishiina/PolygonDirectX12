struct Input_PS
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

float4 main(Input_PS input) : SV_TARGET
{
    return input.color;
}