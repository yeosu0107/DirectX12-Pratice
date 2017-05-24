struct VS_INPUT { 
	float3 position : POSITION; 
	float4 color : COLOR; 
};
//정점 셰이더의 출력(픽셀 셰이더의 입력)을 위한 구조체를 선언한다. 
struct VS_OUTPUT { 
	float4 position : SV_POSITION; 
	float4 color : COLOR; 
};
//정점 셰이더를 정의한다. 
VS_OUTPUT VSMain(VS_INPUT input) { 
	VS_OUTPUT output;
	output.position = float4(input.position, 1.0f); 
	output.color = input.color;
	return(output);
}
//픽셀 셰이더를 정의한다. 
float4 PSMain(VS_OUTPUT input) : SV_TARGET { 
	return(input.color); 
}