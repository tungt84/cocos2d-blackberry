/*
 * Smaato.cpp
 *
 *  Created on: Mar 2, 2016
 *      Author: trantung
 */

#include <extensions/smaato/Smaato.h>
#ifdef TIZEN
#include <app.h>
#include <dlog.h>
NS_CC_BEGIN
#define TAG "MY_TAG"
// Callback function to get result
static void app_control_result(app_control_h request, app_control_h reply, app_control_result_e result, void *user_data)
{
   char *value;

   if (result == APP_CONTROL_RESULT_SUCCEEDED)
   {
      if (app_control_get_extra_data(reply, APP_CONTROL_DATA_SELECTED, &value) == APP_CONTROL_ERROR_NONE)
      {
         dlog_print(DLOG_INFO, TAG, "[app_control_result_cb] Succeeded: value(%s)", value);
      }
      else
      {
         dlog_print(DLOG_ERROR, TAG, "[app_control_result_cb] Failed");
      }

   }
   else
   {
      dlog_print(DLOG_ERROR, TAG, "[app_control_result_cb] APP_CONTROL_RESULT_FAILED.");
   }
}
void Smaato::openUrl(const char* target){
	app_control_h app_control;
	app_control_create(&app_control);
	app_control_set_operation(app_control, APP_CONTROL_OPERATION_VIEW);
	app_control_set_uri(app_control, target);
	app_control_set_mime(app_control,"application/xhtml+xml");
	//app_control_set_app_id(app_control,"com.samsung.browser");
	int rs =0;
	rs =app_control_send_launch_request(app_control, app_control_result, NULL);
	if ( rs== APP_CONTROL_ERROR_NONE)
	{
	   dlog_print(DLOG_INFO, TAG, "Succeeded: the application is launched.");
	}
	else
	{
	   dlog_print(DLOG_ERROR, TAG, "Failed to launch an application. %d",rs);
	}

	app_control_destroy(app_control);
}
NS_CC_END
#endif
