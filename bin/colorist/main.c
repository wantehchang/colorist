// ---------------------------------------------------------------------------
//                         Copyright Joe Drago 2018.
//         Distributed under the Boost Software License, Version 1.0.
//            (See accompanying file LICENSE_1_0.txt or copy at
//                  http://www.boost.org/LICENSE_1_0.txt)
// ---------------------------------------------------------------------------

#include "colorist/colorist.h"

int main(int argc, char * argv[])
{
    int ret = 1;
    clContext * C = clContextCreate(NULL);

    if (!clContextParseArgs(C, argc, argv)) {
        return 1;
    }
    if (C->help || (C->action == CL_ACTION_NONE)) {
        clContextPrintSyntax(C);
        clContextDestroy(C);
        return 0;
    }
    if (C->verbose)
        clContextPrintArgs(C);

    switch (C->action) {
        case CL_ACTION_CONVERT:
            ret = clContextConvert(C);
            break;
        case CL_ACTION_GENERATE:
            ret = clContextGenerate(C);
            break;
        case CL_ACTION_IDENTIFY:
            ret = clContextIdentify(C);
            break;
        default:
            clContextLogError(C, "Unimplemented action: %s", clActionToString(C, C->action));
            break;
    }
    clContextDestroy(C);
    return ret;
}
