

float DeCasteljeu(float coef[4], float t, int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        int currentSize = n - i;
        for (int j = 0; j < currentSize - 1; j++)
        {
            coef[j] = coef[j] * (1.0f - t) + coef[j + 1] * t;
        }
    }
    float value = coef[0];
    
    return value;
}