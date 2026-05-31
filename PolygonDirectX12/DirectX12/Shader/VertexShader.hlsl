struct Input_VS
{
    float3 pos : POSITION;
    uint index : SV_VertexID;
};

struct Output_VS
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

StructuredBuffer<float4> Colors : register(t0);

Output_VS main(Input_VS input)
{
    Output_VS vs;
    vs.pos = float4(input.pos,1);
    vs.color = Colors[input.index];
    //vs.color = float4(1, 1, 1, 1);
	return vs;
}