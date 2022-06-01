#version 430 core


in vec2 texCoord;
flat in int tex;

out vec4 outColor;

layout(location = 0) uniform sampler2D whKing;
layout(location = 1) uniform sampler2D whQueen;
layout(location = 2) uniform sampler2D whRook;
layout(location = 3) uniform sampler2D whKnight;
layout(location = 4) uniform sampler2D whBishop;
layout(location = 5) uniform sampler2D whPawn;
layout(location = 6) uniform sampler2D bKing;
layout(location = 7) uniform sampler2D bQueen;
layout(location = 8) uniform sampler2D bRook;
layout(location = 9) uniform sampler2D bKnight;
layout(location = 10)uniform sampler2D bBishop;
layout(location = 11)uniform sampler2D bPawn;
layout(location = 12)uniform sampler2D selection;
layout(location = 13)uniform sampler2D col2;
layout(location = 14)uniform sampler2D capture;
layout(location = 15)uniform sampler2D evolvebg;


void main(){
	switch(tex){
		case 0:
			outColor = texture(whKing, texCoord);
			break;
		case 1:
			outColor = texture(whQueen, texCoord);
			break;
		case 2:
			outColor = texture(whRook, texCoord);
			break;
		case 3:
			outColor = texture(whKnight, texCoord);
			break;
		case 4:
			outColor = texture(whBishop, texCoord);
			break;
		case 5:
			outColor = texture(whPawn, texCoord);
			break;
		case 6:
			outColor = texture(bKing, texCoord);
			break;
		case 7:
			outColor = texture(bQueen, texCoord);
			break;
		case 8:
			outColor = texture(bRook, texCoord);
			break;
		case 9:
			outColor = texture(bKnight, texCoord);
			break;
		case 10:
			outColor = texture(bBishop, texCoord);
			break;
		case 11:
			outColor = texture(bPawn, texCoord);
			break;
		case 12:
			outColor = texture(selection, texCoord);
			break;
		case 13:
			outColor = texture(col2, texCoord);
			break;
		case 14:
			outColor = texture(capture, texCoord);
			break;
		case 15:
			outColor = texture(evolvebg, texCoord);
			break;
		default:
			outColor = vec4(1);
			break;
	}

	//outColor = vec4(vec3(float(tex)), 1);
	
}