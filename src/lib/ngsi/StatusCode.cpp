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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>

#include "logMsg/logMsg.h"

#include "common/globals.h"
#include "common/string.h"
#include "common/tag.h"
#include "common/Format.h"
#include "ngsi/Request.h"
#include "ngsi/StatusCode.h"
#include "ngsi10/UpdateContextResponse.h"
#include "rest/HttpStatusCode.h"



/* ****************************************************************************
*
* StatusCode::StatusCode -
*/
StatusCode::StatusCode()
{
  code         = SccNone;
  reasonPhrase = "";
  details      = "";
  tag          = "statusCode";
}



/* ****************************************************************************
*
* StatusCode::StatusCode -
*/
StatusCode::StatusCode(const std::string& _tag)
{
  code         = SccNone;
  reasonPhrase = "";
  details      = "";
  tag          = _tag;
}



/* ****************************************************************************
*
* StatusCode::StatusCode -
*/
StatusCode::StatusCode(HttpStatusCode _code, const std::string& _details, const std::string& _tag)
{
  code          = _code;
  reasonPhrase  = httpStatusCodeString(code);
  details       = _details;
  tag           = _tag;
}



/* ****************************************************************************
*
* StatusCode::render -
*/
std::string StatusCode::render(Format format, const std::string& indent, bool comma, bool showTag)
{
  std::string  out  = "";

  if (strstr(details.c_str(), "\"") != NULL)
  {
    int    len = details.length() * 2;
    char*  s2    = (char*) calloc(1, len + 1);

    strReplace(s2, len, details.c_str(), "\"", "\\\"");
    details = s2;
    free(s2);
  }

  if (code == SccNone)
  {
    fill(SccReceiverInternalError, "");
    details += " - ZERO code set to 500";
  }

  out += startTag(indent, tag, format, showTag);
  out += valueTag(indent + "  ", "code", code, format, true);
  out += valueTag(indent + "  ", "reasonPhrase", reasonPhrase, format, details != "");

  if (details != "")
  {
    out += valueTag(indent + "  ", "details", details, format, false);
  }

  out += endTag(indent, tag, format, comma);

  return out;
}



/* ****************************************************************************
*
* StatusCode::fill -
*/
void StatusCode::fill(HttpStatusCode _code, const std::string& _details)
{
  code          = _code;
  reasonPhrase  = httpStatusCodeString(code);
  details       = _details;
}



/* ****************************************************************************
*
* StatusCode::fill -
*/
void StatusCode::fill(StatusCode* scP)
{
  fill(scP->code, scP->details);
}



/* ****************************************************************************
*
* StatusCode::fill -
*/
void StatusCode::fill(const StatusCode& sc)
{
  fill(sc.code, sc.details);
}



/* ****************************************************************************
*
* StatusCode::fill - 
*/
void StatusCode::fill(const struct UpdateContextResponse& ucrs)
{
  if ((ucrs.errorCode.code != SccOk) && (ucrs.errorCode.code != SccNone))
  {
    fill(ucrs.errorCode);
  }
  else if (ucrs.contextElementResponseVector.vec.size() == 1)
  {
    fill(ucrs.contextElementResponseVector.vec[0]->statusCode);
  }
  else if (ucrs.contextElementResponseVector.vec.size() > 1)
  {
    LM_W(("Filling StatusCode from UpdateContextResponse with more than one contextElementResponse, picking one of them ..."));
    fill(ucrs.contextElementResponseVector.vec[0]->statusCode);
  }
  else
  {
    // Empty UpdateContextResponse::contextElementResponseVector AND unfilled UpdateContextResponse::errorCode
    LM_E(("Internal Error (can't fill StatusCode from UpdateContextResponse)"));
    fill(SccReceiverInternalError, "can't fill StatusCode from UpdateContextResponse");
  }
}



/* ****************************************************************************
*
* StatusCode::check -
*/
std::string StatusCode::check
(
  RequestType         requestType,
  Format              format,
  const std::string&  indent,
  const std::string&  predetectedError,
  int                 counter
)
{
  if (code == SccNone)
  {
    return "no code";
  }

  if (reasonPhrase == "")
  {
    return "no reason phrase";
  }

  return "OK";
}



/* ****************************************************************************
*
* StatusCode::present -
*/
void StatusCode::present(const std::string& indent)
{
  LM_F(("%s%s:", indent.c_str(), tag.c_str()));
  LM_F(("%s  Code:            %d",   indent.c_str(), code));
  LM_F(("%s  ReasonPhrase:    '%s'", indent.c_str(), reasonPhrase.c_str()));
  LM_F(("%s  Detail:          '%s'", indent.c_str(), details.c_str()));
}



/* ****************************************************************************
*
* release -
*/
void StatusCode::release(void)
{
  code         = SccNone;
  reasonPhrase = "";
  details      = "";
  tag          = "statusCode";
}



/* ****************************************************************************
*
* tagSet -
*/
void StatusCode::tagSet(const std::string& _tag)
{
  tag = _tag;
}
