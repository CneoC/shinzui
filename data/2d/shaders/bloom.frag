uniform sampler2D source;

void main()
{
	vec4 sum = vec4(0);
	vec2 texcoord = vec2(gl_TexCoord[0]);
	int j;
	int i;

	for (i = -4; i < 4; i++)
	{
		for (j = -3; j < 3; j++)
		{
			sum += texture2D(source, texcoord + vec2(j, i) * 0.004) * 0.25;
		}
	}
	
	sum *= sum;
	if (texture2D(source, texcoord).r < 0.3)
	{
		gl_FragColor = sum * 0.012 + texture2D(source, texcoord);
	}
	else
	{
		if (texture2D(source, texcoord).r < 0.5)
		{
			gl_FragColor = sum * 0.009 + texture2D(source, texcoord);
		}
		else
		{
			gl_FragColor = sum * 0.0075 + texture2D(source, texcoord);
		}
	}
}
