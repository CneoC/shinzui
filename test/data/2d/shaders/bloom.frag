uniform sampler2D source;

const float checkOffset = 0.003;
const int checks = 4;
const float amount = 16.0;

void main()
{
	vec4 sum = vec4(0);

	vec2 uv = gl_TexCoord[0].st;
	
	float maxOffset = checkOffset * (checks + 1);
	for (int y = -checks; y <= checks; y++)
	{
		float mul = (maxOffset - checkOffset);
		sum += texture2D(source, uv + vec2(0, float(y) * checkOffset)) * mul;
	}

	for (int x = -checks; x <= checks; x++)
	{
		float mul = (maxOffset - checkOffset);
		sum += texture2D(source, uv + vec2(float(x) * checkOffset, 0)) * mul;
	}
	sum *= amount;

	gl_FragColor = sum;
}
