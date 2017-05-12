#include <stdlib.h>
#include <stdio.h>
#include "ig_base.h"

#define HANDMADE_MATH_IMPLEMENTATION
#define HANDMADE_MATH_CPP_MODE
#include "HandmadeMath.h"

#include "ig_util.cpp"
#include "ig_tokenizer.cpp"

#define MIN_WELD_LENGTH 1.50f

internal void
printVec3(hmm_vec3 v3, b32 newline = TRUE)
{
    printf("X:%.3f, Y:%.3f, Z:%.3f", v3.X, v3.Y, v3.Z);
    if (newline)
        printf("\n");
}


internal r32
roundSixteenths(r32 input)
{
    input += 1/32.0f;
    s64 whole = (s64)input;
    s64 frac = (s64)((input - (r32)whole) * 16);
    r32 result = whole + frac/16.0f;
    return(result);
}

internal r32
roundFrac(r32 input, u32 fraction)
{
    Assert(fraction > 0);
    input += 1.0f/(2*fraction);
    s64 whole = (s64)input;
    s64 frac = (s64)((input - (r32)whole) * fraction);
    r32 result = whole + frac/(r32)(fraction);
    return(result);   
}

internal void
computePatternsWithCoverage(r32 weldDistance, r32 weldCoverageTarget, u32 precision=8)
{
    printf("Computing Stitch Weld Paterns for Total Distance: %0.3f, with coverage of: %0.2f%% rounded to fractional values:%d\n", 
        weldDistance, weldCoverageTarget*100, precision);
    
    r32 stitchDistance = weldDistance - MIN_WELD_LENGTH;
    if (stitchDistance <= MIN_WELD_LENGTH)
    {
        if (weldCoverageTarget <= 0.5f)
        {
            printf("Length:%0.2f\n", MIN_WELD_LENGTH); 
        }
        else
        {
            r32 suggestedWeld = weldCoverageTarget*weldDistance;
            suggestedWeld = roundFrac(suggestedWeld, precision);
            if (suggestedWeld < MIN_WELD_LENGTH)
                suggestedWeld = MIN_WELD_LENGTH;
            printf("Length:%0.2f\n", suggestedWeld);    
        }
    }
    else
    {
        for (u32 stitchN = (u32)(weldDistance / MIN_WELD_LENGTH); 
                stitchN > 1; 
                --stitchN)
        {
            r32 stitchL = weldCoverageTarget * weldDistance / stitchN;
            stitchL = roundFrac(stitchL, precision);
            
            if ((stitchL < MIN_WELD_LENGTH) && (stitchN > 2))
            {
                 continue;
            }   
            else if (stitchL < MIN_WELD_LENGTH)
            {
                stitchL = MIN_WELD_LENGTH;
            }
            
            r32 stitchDistance = weldDistance - stitchL;
            
            r32 stitchP = stitchDistance / (r32)(stitchN - 1);
            stitchP = roundFrac(stitchP, precision);

            r32 actualCoverage = stitchN * stitchL / weldDistance;
            printf("Weld (Length - Pitch):%0.4f - %0.4f\t\tStitches:%d\tCoverage:%0.2f%%\n", 
                stitchL, stitchP, stitchN, actualCoverage*100);
        }
    }
}

internal void
outputUseage()
{
    printf("Stitchcalc Useage: stitchcalc WeldDistance weldCoverageTargetPercentage[0.0..1)\n");
}

int
main(int argc, char **args)
{

    if(argc < 3)
    {
        outputUseage();
    }
    else if(argc <= 4)
    {
        r32 WeldDistance = (r32)atof(args[1]);
        r32 WeldCoverageTarget = (r32)atof(args[2]);
        u32 precision = 8;
        if (argc >= 4)
        {
            precision = atoi(args[3]);
        }
        computePatternsWithCoverage(WeldDistance, WeldCoverageTarget, precision);
    }
    else
    {
        printf("Argument error.\n");
    }
    
    return 0;
}
