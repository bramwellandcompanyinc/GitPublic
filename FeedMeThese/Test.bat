rem This file assumes a VS2010 solution.  If this is compiled in a VS2013 environment the path below will need to be changed to reflect the location of MSTest.exe
rem This file will operate if executed from a VS command prompt or if the script is executed from the solution folder.

rem VS2010

"C:\Program Files (x86)\Microsoft Visual Studio 10.0\Common7\IDE\mstest.exe" /testcontainer:ProcessorTest\bin\debug\processortest.dll

rem VS2013

rem "C:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\IDE\mstest.exe" /testcontainer:ProcessorTest\bin\debug\processortest.dll