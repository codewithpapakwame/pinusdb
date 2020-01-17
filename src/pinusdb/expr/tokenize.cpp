/*
* Copyright (c) 2019 ChangSha JuSong Soft Inc. <service@pinusdb.cn>.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; version 3 of the License.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.

* You should have received a copy of the GNU General Public License
* along with this program; If not, see <http://www.gnu.org/licenses>
*/

#include <ctype.h>
#include <stdlib.h>
#include "expr/parse.h"
#include "expr/tokenize.h"
#include "expr/pdb_db_int.h"
#include "expr/sql_parser.h"
#include "pdb_error.h"

void* pdbParseAlloc(void*(*)(size_t));
void pdbParseFree(void *p, void(*freeProc)(void*));
void pdbParse(void*, int, Token, SQLParser*);


typedef struct Keyword Keyword;
struct Keyword {
  const char* zName;           // The keyword name
  int tokenType;
};

/*
** If X is a character that can be used in an identifier and
** X&0x80==0 then isIdChar[X] will be 1.  If X&0x80==0x80 then
** X is always an identifier character.  (Hence all UTF-8
** characters can be part of an identifier).  isIdChar[X] will
** be 0 for every character in the lower 128 ASCII characters
** that cannot be used as part of an identifier.
**
** In this implementation, an identifier can be a string of
** alphabetic characters, digits, and "_" plus any character
** with the high-order bit set.  The latter rule means that
** any sequence of UTF-8 characters or characters taken from
** an extended ISO8859 character set can form an identifier.
*/
static const char isIdChar[] = {
  /* x0 x1 x2 x3 x4 x5 x6 x7 x8 x9 xA xB xC xD xE xF */
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  /* 0x */
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  /* 1x */
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  /* 2x */
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,  /* 3x */
  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  /* 4x */
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1,  /* 5x */
  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  /* 6x */
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,  /* 7x */
};

static const uint8_t char2blob[] = {
  /* 0     1     2     3     4     5     6     7     8    9      A     B     C     D     E     F */
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, /*0x*/
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, /*1x*/
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, /*2x*/
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, /*3x*/
  0xFF, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, /*4x*/
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, /*5x*/
  0xFF, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, /*6x*/
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, /*7x*/
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, /*8x*/
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, /*9x*/
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, /*Ax*/
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, /*Bx*/
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, /*Cx*/
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, /*Dx*/
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, /*Ex*/
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, /*Fx*/
};

static Keyword aKeywordTable[] = {
  { "AND",               TK_AND },
  { "AS",                TK_AS },
  { "DROP",              TK_DROP },
  { "FROM",              TK_FROM },
  { "INSERT",            TK_INSERT },
  { "INTO",              TK_INTO },
  { "IS",                TK_IS },
  { "NOT",               TK_NOT },
  { "NULL",              TK_NULL },
  { "SELECT",            TK_SELECT },
  { "TABLE",             TK_TABLE },
  { "VALUES",            TK_VALUES },
  { "WHERE",             TK_WHERE },
  { "CREATE",            TK_CREATE },
  { "GROUP",             TK_GROUP },
  { "BY",                TK_BY },
  { "LIKE",              TK_LIKE },
  { "ADD",               TK_ADD },
  { "USER",              TK_USER },
  { "IDENTIFIED",        TK_IDENTIFIED },
  { "ROLE",              TK_ROLE },
  { "SET",               TK_SET },
  { "PASSWORD",          TK_PASSWORD },
  { "FOR",               TK_FOR },
  { "BOOL",              TK_BOOL_TYPE},
  { "BIGINT",            TK_BIGINT_TYPE},
  { "DATETIME",          TK_DATETIME_TYPE},
  { "DOUBLE",            TK_DOUBLE_TYPE},
  { "STRING",            TK_STRING_TYPE},
  { "BLOB",              TK_BLOB_TYPE},
  { "REAL2",             TK_REAL2_TYPE},
  { "REAL3",             TK_REAL3_TYPE},
  { "REAL4",             TK_REAL4_TYPE},
  { "REAL6",             TK_REAL6_TYPE},
  { "ORDER",             TK_ORDER},
  { "DESC",              TK_DESC},
  { "ASC",               TK_ASC},
  { "ATTACH",            TK_ATTACH},
  { "DETACH",            TK_DETACH},
  { "IN",                TK_IN},
  { "LIMIT",             TK_LIMIT},
  { "TRUE",              TK_TRUE},
  { "FALSE",             TK_FALSE},
  { "DATAFILE",          TK_DATAFILE},

  { "DELETE",            TK_DELETE},
  { "TOP",               TK_TOP},
  { "ALTER",             TK_ALTER}
};

std::unordered_map<uint64_t, int> Tokenize::keywordMap_;

void Tokenize::InitTokenize()
{
  if (keywordMap_.size() == 0)
  {
    int keywordCnt = sizeof(aKeywordTable) / sizeof(aKeywordTable[0]);
    for (int i = 0; i < keywordCnt; i++)
    {
      uint64_t keyCode = StringTool::CRC64NoCase(aKeywordTable[i].zName);
      std::pair<uint64_t, int> pairKey(keyCode, aKeywordTable[i].tokenType);
      keywordMap_.insert(pairKey);
    }
  }
}

PdbErr_t Tokenize::RunParser(Arena* pArena, SQLParser* pParse, const char* pSql, size_t sqlLen)
{
  void* pEngine = pdbParseAlloc(malloc);
  if (pEngine == nullptr)
  {
    pParse->SetError();
    return PdbE_NOMEM;
  }

  PdbErr_t retVal = PdbE_OK;

  char* pDestStr = nullptr;
  size_t destLen = 0;
  bool needEscape = true;

  int tokenType = 0;
  size_t i = 0;
  Token lastToken;

  while (pSql[i] != '\0' && i < sqlLen && !pParse->GetError())
  {
    needEscape = false;
    lastToken.str_ = &pSql[i];
    lastToken.len_ = GetToken((const unsigned char*)&pSql[i], (sqlLen - i), &tokenType, &needEscape);
    i += lastToken.len_;

    if (tokenType == TK_STRING)
    {
      if (needEscape)
      {
        pDestStr = pArena->Allocate(lastToken.len_);
        if (pDestStr == nullptr)
          return PdbE_NOMEM;

        retVal = GetStr(lastToken.str_, lastToken.len_, pDestStr, &destLen);
        if (retVal != PdbE_OK)
          return retVal;

        //�ַ���ʹ��ʱ�������ó�����Ϊ���������Դ˴���������ĩβΪ0
        lastToken.str_ = pDestStr;
        lastToken.len_ = (int)destLen;
      }
      else
      {
        lastToken.str_++;
        lastToken.len_ -= 2;
      }
    }
    else if (tokenType == TK_BLOB) 
    {
      if (lastToken.len_ % 2 == 0)
        return PdbE_INVALID_BLOB_VAL;

      if (lastToken.len_ >= 5)
      {
        pDestStr = pArena->Allocate(((lastToken.len_ - 3) / 2));
        if (pDestStr == nullptr)
          return PdbE_NOMEM;

        retVal = StrToBlob(lastToken.str_, lastToken.len_, (uint8_t*)pDestStr, &destLen);
        if (retVal != PdbE_OK)
          return retVal;

        lastToken.str_ = pDestStr;
        lastToken.len_ = (int)destLen;
      }
      else
      {
        lastToken.str_ = nullptr;
        lastToken.len_ = 0;
      }
    }

    switch (tokenType)
    {
    case TK_SPACE:
    case TK_COMMENT:
    break;
    case TK_ILLEGAL:
    {
      pParse->SetError();
    }
    break;
    default: {
      pdbParse(pEngine, tokenType, lastToken, pParse);
      break;
    }
    }
  }

  if (i > sqlLen)
  {
    pParse->SetError();
  }

  if (!pParse->GetError())
  {
    lastToken.str_ = nullptr;
    lastToken.len_ = 0;
    if (tokenType != TK_SEMI)
    {
      pdbParse(pEngine, TK_SEMI, lastToken, pParse);
    }
    pdbParse(pEngine, 0, lastToken, pParse);

    pdbParseFree(pEngine, free);
    return PdbE_OK;
  }

  pdbParseFree(pEngine, free);

  return PdbE_OK;
}

PdbErr_t Tokenize::StrToBlob(const char* pSourceStr, size_t sourceLen, uint8_t* pDest, size_t* pDestLen)
{
  size_t destLen = (sourceLen - 3) / 2;
  
  uint8_t c = 0;
  
  const uint8_t* pTmpSource = (const uint8_t*)(pSourceStr + 2); //����ǰ��� x'

  for (size_t pos = 0; pos < destLen; pos++)
  {
    c = 0;

    if (char2blob[*pTmpSource] != 0xFF)
    {
      c = char2blob[*pTmpSource] << 4;
    }
    else
    {
      return PdbE_INVALID_BLOB_VAL;
    }

    pTmpSource++;

    if (char2blob[*pTmpSource] != 0xFF)
    {
      c += char2blob[*pTmpSource];
    }
    else
    {
      return PdbE_INVALID_BLOB_VAL;
    }

    pTmpSource++;

    pDest[pos] = c;
  }

  if (pDestLen != nullptr)
    *pDestLen = destLen;

  return PdbE_OK;
}

PdbErr_t Tokenize::GetStr(const char* pSourceStr, size_t sourceLen, char* pDest, size_t* pDestLen)
{
  size_t idx = 0;

  char delim = *pSourceStr;

  for (size_t pos = 1; pos < sourceLen - 1; pos++)
  {
    if (pSourceStr[pos] == delim)
    {
      if (pSourceStr[pos + 1] == delim)
      {
        pDest[idx++] = pSourceStr[pos];
        pos++;
      }
      else
        break;
    }
    else
    {
      pDest[idx++] = pSourceStr[pos];
    }
  }

  if (pDestLen != nullptr)
    *pDestLen = idx;

  return PdbE_OK;
}


size_t Tokenize::GetToken(const unsigned char* z, size_t maxLen, int* tokenType, bool* needEscape)
{
  size_t i;
  switch (*z) {
  case ' ': case '\t': case '\n': case '\f': case '\r': {
    for (i = 1; i < maxLen && isspace(z[i]); i++) {}
    *tokenType = TK_SPACE;
    return i;
  }
  case '-': {
    if (z[1] == '-') {
      for (i = 2; i < maxLen && z[i] && z[i] != '\n'; i++) {}
      *tokenType = TK_COMMENT;
      return i;
    }
    *tokenType = TK_MINUS;
    return 1;
  }
  case '(': {
    *tokenType = TK_LP;
    return 1;
  }
  case ')': {
    *tokenType = TK_RP;
    return 1;
  }
  case ';': {
    *tokenType = TK_SEMI;
    return 1;
  }
  case '+': {
    *tokenType = TK_PLUS;
    return 1;
  }
  case '*': {
    *tokenType = TK_STAR;
    return 1;
  }
  case '=': {
    *tokenType = TK_EQ;
    if (maxLen > 1)
    {
      return 1 + (z[1] == '=' ? 1 : 0);
    }

    return 1;
  }
  case '<': {
    if (maxLen > 1)
    {
      if (z[1] == '=') {
        *tokenType = TK_LE;
        return 2;
      }
      else if (z[1] == '>') {
        *tokenType = TK_NE;
        return 2;
      }
    }

    *tokenType = TK_LT;
    return 1;
  }
  case '>': {
    if (maxLen > 1 && z[1] == '=') {
      *tokenType = TK_GE;
      return 2;
    }
    else {
      *tokenType = TK_GT;
      return 1;
    }
  }
  case '!': {
    if (maxLen > 1 && z[1] == '=')
    {
      *tokenType = TK_NE;
      return 2;
    }
    else
    {
      *tokenType = TK_ILLEGAL;
      return 1;
    }
  }
  case ',': {
    *tokenType = TK_COMMA;
    return 1;
  }
  case '\'': case '"': {
    int delim = z[0];
    for (i = 1; z[i] && i < maxLen; i++) {
      if (z[i] == delim) {
        if ((i + 1) < maxLen && z[i + 1] == delim) {
          i++;
          if (needEscape != nullptr) *needEscape = true;
        }
        else {
          break;
        }
      }
    }
    if (i < maxLen && z[i] == delim)
    {
      i++;
      *tokenType = TK_STRING;
    }
    else
    {
      *tokenType = TK_ILLEGAL;
    }
    return i;
  }
  case '0': case '1': case '2': case '3': case '4':
  case '5': case '6': case '7': case '8': case '9': {
    *tokenType = TK_INTEGER;
    for (i = 1; i < maxLen && isdigit(z[i]); i++) {}
    if ((i + 1) < maxLen && z[i] == '.' && isdigit(z[i + 1])) {
      i += 2;
      while (i < maxLen && isdigit(z[i])) { i++; }
      *tokenType = TK_DOUBLE;
    }
    if ((i + 2) < maxLen && (z[i] == 'e' || z[i] == 'E') &&
      (isdigit(z[i + 1]) || ((z[i + 1] == '+' || z[i + 1] == '-') && isdigit(z[i + 2])))
      ) {
      i += 2;
      while (i < maxLen && isdigit(z[i])) { i++; }
      *tokenType = TK_DOUBLE;
    }
    return i;
  }
  case '[': {
    for (i = 1; i < maxLen && z[i] && z[i - 1] != ']'; i++) {}
    *tokenType = TK_ID;
    return i;
  }
  case 'x': case 'X': {
    if (maxLen > 1 && (z[1] == '\'' || z[1] == '"')) {
      int delim = z[1];
      *tokenType = TK_BLOB;
      for (i = 2; i < maxLen && z[i]; i++) {
        if (z[i] == delim) {
          if (i % 2) *tokenType = TK_ILLEGAL;
          break;
        }
        if (!isxdigit(z[i])) {
          *tokenType = TK_ILLEGAL;
          return i;
        }
      }
      if (i < maxLen && z[i] == delim)
      {
        i++;
      }
      return i;
    }
    /* Otherwise fall through to the next case */
  }
  default: {
    if ((*z & 0x80) == 0 && !isIdChar[*z]) {
      break;
    }
    for (i = 1; i < maxLen && ((z[i] & 0x80) != 0 || isIdChar[z[i]] || z[i] == '.'); i++) {}
    *tokenType = GetKeywordType((char*)z, i);
    return i;
  }
  }
  *tokenType = TK_ILLEGAL;
  return 1;
}


int Tokenize::GetKeywordType(const char* z, size_t n)
{
  const size_t maxKeyLen = 32;

  if (n >= maxKeyLen)
    return TK_ID;

  uint64_t tmpKey = StringTool::CRC64NoCase(z, n);

  auto keyIter = keywordMap_.find(tmpKey);
  if (keyIter != keywordMap_.end())
  {
    return keyIter->second;
  }
  return TK_ID;
}

