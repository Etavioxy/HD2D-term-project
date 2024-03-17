#version 150

uniform sampler2D positionTexture;
uniform sampler2D focusTexture;
uniform sampler2D outOfFocusTexture;

uniform vec3 FocusPoint;
uniform vec2 nearFar;

out vec4 fragColor;
out vec4 fragColor1;

void main() {
  float minDistance =  3.0;
  float maxDistance = 7.5;

  float far  = nearFar.y;

  vec2 texSize  = textureSize(focusTexture, 0).xy;
  vec2 texCoord = gl_FragCoord.xy / texSize;

  vec4 focusColor = texture(focusTexture, texCoord);

  fragColor = focusColor;

  vec4 position = texture(positionTexture, texCoord);

  if (position.a <= 0) { fragColor1 = vec4(1.0); return; }

  vec4 outOfFocusColor = texture(outOfFocusTexture, texCoord);
  vec4 focusPoint      = vec4(FocusPoint, 1.0);

  float blur =
    smoothstep
      ( minDistance
      , maxDistance
      , length(position - focusPoint)
      );

  fragColor  = mix(focusColor, outOfFocusColor, blur);
  fragColor1 = vec4(blur);
}
