#include "endpoints.h"
#include "settings.h"
#include "asyncBackend.h"
#include "httpRequest.h"






//..............................................................LOCAL

void on_global(HttpRequest& request)
{

}

//.............................................................HEADER

void endpoints_init()
{
    asyncBackend_register_endpoint("esp/glob", on_global);
}