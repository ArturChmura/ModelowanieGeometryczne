

float DeBoor(float coef[4], float x)
{
    int k = 3;
    float t[8] = { -3,-2,-1, 0, 1, 2,3,4};
    int p = 3;
    
    float d[4] = { coef[0], coef[1], coef[2], coef[3] };
    for (int r = 1; r <= p ; r++) // r in <1,4>
    {
        for (int j = p; j >= r; j--) // j in <3,0> <3,1> <3,2> <3,3>
        {
            float alpha = (x - t[j + k - p]) / (t[j + 1 + k - r] - t[j + k - p]);
            d[j] = (1.0f - alpha) * d[j - 1] + alpha * d[j];
        }

    }
    return d[3];
}