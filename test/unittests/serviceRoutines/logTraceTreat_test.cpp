/*
*
* Copyright 2013 Telefonica Investigacion y Desarrollo, S.A.U
*
* This file is part of Orion Context Broker.
*
* Orion Context Broker is free software: you can redistribute it and/or
* modify it under the terms of the GNU Affero General Public License as
* published by the Free Software Foundation, either version 3 of the
* License, or (at your option) any later version.
*
* Orion Context Broker is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero
* General Public License for more details.
*
* You should have received a copy of the GNU Affero General Public License
* along with Orion Context Broker. If not, see http://www.gnu.org/licenses/.
*
* For those usages not covered by this license please contact with
* iot_support at tid dot es
*
* Author: Ken Zangelin
*/
#include "serviceRoutines/logTraceTreat.h"
#include "serviceRoutines/badRequest.h"
#include "rest/RestService.h"

#include "unittest.h"



/* ****************************************************************************
*
* rs - 
*/
static RestService rs[] = 
{
  { "GET",    LogRequest,                            2, { "log", "traceLevel"                                  }, "", logTraceTreat                             },
  { "PUT",    LogRequest,                            3, { "log", "traceLevel", "*"                             }, "", logTraceTreat                             },
  { "POST",   LogRequest,                            3, { "log", "traceLevel", "*"                             }, "", logTraceTreat                             },
  { "DELETE", LogRequest,                            2, { "log", "traceLevel"                                  }, "", logTraceTreat                             },
  { "DELETE", LogRequest,                            3, { "log", "traceLevel", "*"                             }, "", logTraceTreat                             },

  { "*",      InvalidRequest,                        0, { "*", "*", "*", "*", "*", "*"                         }, "", badRequest                                },
  { "",       InvalidRequest,                        0, {                                                      }, "", NULL                                      }
};



/* ****************************************************************************
*
* get - 
*/
TEST(logTraceTreat, get)
{
  ConnectionInfo ci("/log/traceLevel",  "GET", "1.1");
  const char*    outfile = "orion.logTrace.empty.valid.xml";
  std::string    out;

  utInit();

  lmTraceSet(NULL);

  ci.outFormat = XML;
  out          = restService(&ci, rs);
  EXPECT_EQ("OK", testDataFromFile(expectedBuf, sizeof(expectedBuf), outfile)) << "Error getting test data from '" << outfile << "'";
  EXPECT_STREQ(expectedBuf, out.c_str());

  utExit();
}



/* ****************************************************************************
*
* put - 
*/
TEST(logTraceTreat, put)
{
  ConnectionInfo  ci1("/log/traceLevel/0-19,21-200",  "PUT", "1.1");
  ConnectionInfo  ci2("/log/traceLevel/aaa",  "PUT", "1.1");
  ConnectionInfo  ci3("/log/traceLevel",  "GET", "1.1");
  const char*     outfile1 = "orion.logTrace.spanOfLevels.valid.xml";
  const char*     outfile2 = "orion.logTrace.invalidLevels.valid.xml";
  const char*     outfile3 = "orion.logTrace.get.valid.xml";
  std::string     out;

  utInit();

  ci1.outFormat = XML;
  out          = restService(&ci1, rs);
  EXPECT_EQ("OK", testDataFromFile(expectedBuf, sizeof(expectedBuf), outfile1)) << "Error getting test data from '" << outfile1 << "'";
  EXPECT_STREQ(expectedBuf, out.c_str());

  ci2.outFormat = XML;
  out          = restService(&ci2, rs);
  EXPECT_EQ("OK", testDataFromFile(expectedBuf, sizeof(expectedBuf), outfile2)) << "Error getting test data from '" << outfile2 << "'";
  EXPECT_STREQ(expectedBuf, out.c_str());

  ci3.outFormat = XML;
  out          = restService(&ci3, rs);
  EXPECT_EQ("OK", testDataFromFile(expectedBuf, sizeof(expectedBuf), outfile3)) << "Error getting test data from '" << outfile3 << "'";
  EXPECT_STREQ(expectedBuf, out.c_str());

  utExit();
}



/* ****************************************************************************
*
* post - 
*/
TEST(logTraceTreat, post)
{
  ConnectionInfo  ci("/log/traceLevel/20",  "POST", "1.1");
  const char*     outfile = "orion.logTrace.post20.valid.xml";
  std::string     out;

  utInit();

  ci.outFormat = XML;
  out          = restService(&ci, rs);
  EXPECT_EQ("OK", testDataFromFile(expectedBuf, sizeof(expectedBuf), outfile)) << "Error getting test data from '" << outfile << "'";
  EXPECT_STREQ(expectedBuf, out.c_str());

  utExit();
}



/* ****************************************************************************
*
* deleteIndividual - 
*/
TEST(logTraceTreat, deleteIndividual)
{
  ConnectionInfo  ci0("/log/traceLevel/0-255",  "PUT",  "1.1");
  ConnectionInfo  ci1("/log/traceLevel/161",  "DELETE", "1.1");
  ConnectionInfo  ci2("/log/traceLevel/aaa",  "DELETE", "1.1");
  ConnectionInfo  ci3("/log/traceLevel",  "GET", "1.1");
  const char*     outfile0 = "orion.logTrace.allLevelsSet.valid.xml";
  const char*     outfile1 = "orion.logTrace.deleteOk.valid.xml";
  const char*     outfile2 = "orion.logTrace.invalidDelete.valid.xml";
  const char*     outfile3 = "orion.logTrace.deleteIndividual4.valid.xml";
  std::string     out;

  utInit();

  ci0.outFormat = XML;
  out          = restService(&ci0, rs);
  EXPECT_EQ("OK", testDataFromFile(expectedBuf, sizeof(expectedBuf), outfile0)) << "Error getting test data from '" << outfile0 << "'";
  EXPECT_STREQ(expectedBuf, out.c_str());

  ci1.outFormat = XML;
  out           = restService(&ci1, rs);
  EXPECT_EQ("OK", testDataFromFile(expectedBuf, sizeof(expectedBuf), outfile1)) << "Error getting test data from '" << outfile1 << "'";
  EXPECT_STREQ(expectedBuf, out.c_str());

  ci2.outFormat = XML;
  out          = restService(&ci2, rs);
  EXPECT_EQ("OK", testDataFromFile(expectedBuf, sizeof(expectedBuf), outfile2)) << "Error getting test data from '" << outfile2 << "'";
  EXPECT_STREQ(expectedBuf, out.c_str());

  ci3.outFormat = XML;
  out          = restService(&ci3, rs);
  EXPECT_EQ("OK", testDataFromFile(expectedBuf, sizeof(expectedBuf), outfile3)) << "Error getting test data from '" << outfile3 << "'";
  EXPECT_STREQ(expectedBuf, out.c_str());

  utExit();
}



/* ****************************************************************************
*
* deleteAll - 
*/
TEST(logTraceTreat, deleteAll)
{
  ConnectionInfo ci1("/log/traceLevel",  "DELETE", "1.1");
  ConnectionInfo ci2("/log/traceLevel",  "GET", "1.1");
  const char*    outfile1 = "orion.logTrace.allLevelsOff.valid.xml";
  const char*    outfile2 = "orion.logTrace.empty.valid.xml";
  std::string    out;

  utInit();

  ci1.outFormat = XML;
  out           = restService(&ci1, rs);
  EXPECT_EQ("OK", testDataFromFile(expectedBuf, sizeof(expectedBuf), outfile1)) << "Error getting test data from '" << outfile1 << "'";
  EXPECT_STREQ(expectedBuf, out.c_str());

  ci2.outFormat = XML;
  out           = restService(&ci2, rs);
  EXPECT_EQ("OK", testDataFromFile(expectedBuf, sizeof(expectedBuf), outfile2)) << "Error getting test data from '" << outfile2 << "'";
  EXPECT_STREQ(expectedBuf, out.c_str());

  utExit();
}
