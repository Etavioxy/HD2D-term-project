#version 330 core

in vec2 texCoord;

uniform sampler2D texture;

void main()
{             
	vec4 tmp = texture2D( texture, texCoord );

    if (tmp.a < 0.1) {
        discard; // 透明度小于阈值时丢弃片段
    }

    gl_FragDepth = gl_FragCoord.z;
}
