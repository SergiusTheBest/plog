//
// ObjectiveC - shows that plog can be used in ObjectiveC++.
//

#include <objc/Object.h>
#include <plog/Log.h>

@interface Greeter : Object
{
    int dummy;
}
+(void) greet;
@end

@implementation Greeter
+(void) greet
{
    LOGD << "Hello ObjC++!";
}
@end

int main()
{
    plog::init(plog::debug, "ObjectiveC.csv"); // Initialize the logger.
    LOGD << "Hello ObjC++!";
    
    [Greeter greet];
    
    return 0;
}
