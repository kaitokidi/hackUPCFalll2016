uniform sampler2D texture;
uniform vec2 textureSize;
uniform float size;

float isOutline(float alfa) {
	if (alfa > 0.5f) return 0.0f;
	float minDist = 1.0f;
//	float imin,imax,jmin,jmax;
//	imin = imax = jmin = jmax = 0.f;
//	if (texture2D(texture, vec2(gl_TexCoord[0].s-size/textureSize.x,gl_TexCoord[0].t-size/textureSize.y)).w > 0.0f)  imin = jmin = -size;
//	if (texture2D(texture, vec2(gl_TexCoord[0].s+size/textureSize.x,gl_TexCoord[0].t-size/textureSize.y)).w > 0.0f) {imax = size; jmin = -size;}
//	if (texture2D(texture, vec2(gl_TexCoord[0].s-size/textureSize.x,gl_TexCoord[0].t+size/textureSize.y)).w > 0.0f) {imin = -size; jmax = size;}
//	if (texture2D(texture, vec2(gl_TexCoord[0].s+size/textureSize.x,gl_TexCoord[0].t+size/textureSize.y)).w > 0.0f)  imax = jmax = size;
	for (float i = -size; i <= size; ++i) { // the origin is not the top left corner lol
		for (float j = -size; j <= size; ++j) {
			if (texture2D(texture, vec2(gl_TexCoord[0].s+i/textureSize.x,gl_TexCoord[0].t+j/textureSize.y)).w > 0.5f) minDist = min(minDist, sqrt(pow(i/size,2)+pow(j/size,2)));
		}
	}
	if (minDist < 1.0f) return minDist;
	return 0.0f;
}

void main() {
	vec4 originalColor = texture2D(texture,gl_TexCoord[0].st) * gl_Color;
	float outline = isOutline(originalColor.w);
	if (outline > 0.0f) gl_FragColor = vec4(1.0f,1.0f,0.0f,1.0f-outline);
	else discard;
}

// 0.56
