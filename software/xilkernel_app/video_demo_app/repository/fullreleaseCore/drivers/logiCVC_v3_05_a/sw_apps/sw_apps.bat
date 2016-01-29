set APPNAME=logiCVC_demo_lu
rmdir /Q /S %APPNAME%
REM mkdir %APPNAME%\src
REM copy ..\examples\logiCVC_demo.c %APPNAME%\src
REM copy ..\examples\logiCVC_demo.h %APPNAME%\src
REM copy ..\examples\logiCVC_main.c %APPNAME%\src

set APPNAME=logiCVC_demo_stand
mkdir %APPNAME%\src
copy ..\examples\logiCVC_demo.c %APPNAME%\src
copy ..\examples\logiCVC_demo.h %APPNAME%\src
copy ..\examples\logiCVC_main.c %APPNAME%\src
copy ..\examples\SwConfig_cvcdemo.c %APPNAME%\src

