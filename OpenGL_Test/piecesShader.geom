#version 430 core

layout(points) in;

layout(triangle_strip, max_vertices = 4) out;

in int vertTex[];

out vec2 texCoord;
flat out int tex;

uniform float pieceSize;

void main(){
	float truePieceSize = pieceSize;
	int piece = vertTex[0];
	if((piece & ~15) != 0){ // evolving pawns
		piece = (piece & ~15) >> 4;
		piece--;
		tex = 15;
		
		// background
		gl_Position = gl_in[0].gl_Position + vec4(-truePieceSize/2, -truePieceSize/2, 0, 0);
		texCoord = vec2(0,1);
		tex = 15;
		EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4(+truePieceSize/2, -truePieceSize/2, 0, 0);
		texCoord = vec2(1,1);
		tex = 15;
		EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4(-truePieceSize/2, +truePieceSize/2, 0, 0);
		texCoord = vec2(0,0);
		tex = 15;
		EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4(+truePieceSize/2, +truePieceSize/2, 0, 0);
		texCoord = vec2(1,0);
		tex = 15;
		EmitVertex();

		EndPrimitive();
		return;
	}
	if(piece > 11){
		truePieceSize = 0.25;
	}
	gl_Position = gl_in[0].gl_Position + vec4(-truePieceSize/2, -truePieceSize/2, 0, 0);
	texCoord = vec2(0,1);
	tex = piece;
	EmitVertex();
	gl_Position = gl_in[0].gl_Position + vec4(+truePieceSize/2, -truePieceSize/2, 0, 0);
	texCoord = vec2(1,1);
	tex = piece;
	EmitVertex();
	gl_Position = gl_in[0].gl_Position + vec4(-truePieceSize/2, +truePieceSize/2, 0, 0);
	texCoord = vec2(0,0);
	tex = piece;
	EmitVertex();
	gl_Position = gl_in[0].gl_Position + vec4(+truePieceSize/2, +truePieceSize/2, 0, 0);
	texCoord = vec2(1,0);
	tex = piece;
	EmitVertex();

	EndPrimitive();
}