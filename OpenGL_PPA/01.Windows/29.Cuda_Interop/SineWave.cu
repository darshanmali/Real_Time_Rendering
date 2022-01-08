
__global__ void SineWave(float4* pos, unsigned int mess_Width, unsigned int mess_Height, float time)
{
	
   unsigned int x = blockIdx.x * blockDim.x + threadIdx.x;
   unsigned int y = blockIdx.y * blockDim.y + threadIdx.y;

   float u = x / (float)mess_Width;
   float v = y / (float)mess_Height;
   u = u * 2.0f - 1.0f;
   v = v * 2.0f - 1.0f;
   float freq = 4.0f;
   float w = sinf(u * freq + time) + cosf(v * freq + time) * 1.0f;

   pos[y * mess_Width + x] = make_float4(u, w, v, 1.0f);

}

void LaunchCUDAKernal(float4* pos, unsigned int mess_Width, unsigned int mess_Height, float time)
{
	dim3 DimBlock = dim3(8, 8, 1);
	dim3 DimGrid = dim3(mess_Width/ DimBlock.x, mess_Height / DimBlock.y , 1);

	SineWave<<<DimGrid, DimBlock >>> (pos, mess_Width, mess_Height, time);

}


