


float2 GetUV(float u, float v, int flipped, int rowIndex, int columnIndex, int rowCount, int columnCount)
{
    if(flipped)
    {
        float h = u;
        u = v;
        v = h;
    }
        
    u = ((float) rowIndex) / rowCount + u / rowCount;
    v = ((float) columnIndex) / columnCount + v / columnCount;
        
  
    return float2(u, v);
}