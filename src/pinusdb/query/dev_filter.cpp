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

#include "query/dev_filter.h"
#include "util/string_tool.h"
#include "expr/parse.h"
#include "pdb_error.h"
#include "internal.h"

DevFilter::DevFilter()
{
  minDevId_ = 1;
  maxDevId_ = INT64_MAX;
}

DevFilter::~DevFilter()
{
}

PdbErr_t DevFilter::BuildFilter(const ExprItem* pCondition)
{
  minDevId_ = 1;
  maxDevId_ = INT64_MAX;

  return _BuildFilter(pCondition);
}

bool DevFilter::Filter(int64_t devId) const
{
  DBVal vals;
  DBVAL_SET_INT64(&vals, devId);
  for (auto condiIt = conditionVec_.begin(); condiIt != conditionVec_.end(); condiIt++)
  {
    if (!(*condiIt)->GetLogic(&vals, 1))
      return false;
  }
  
  return true;
}

PdbErr_t DevFilter::_BuildFilter(const ExprItem* pExpr)
{
  if (pExpr == nullptr)
    return PdbE_OK;

  PdbErr_t retVal = PdbE_OK;

  int op = pExpr->GetOp();

  const ExprItem* pLeftExpr = pExpr->GetLeftExpr();
  const ExprItem* pRightExpr = pExpr->GetRightExpr();

  if (op == TK_AND)
  {
    if (pLeftExpr == nullptr || pRightExpr == nullptr)
      return PdbE_SQL_CONDITION_EXPR_ERROR;

    retVal = _BuildFilter(pLeftExpr);
    if (retVal != PdbE_OK)
      return retVal;

    retVal = _BuildFilter(pRightExpr);
    if (retVal != PdbE_OK)
      return retVal;

    return retVal;
  }
  else
  {
    if (pLeftExpr == nullptr)
      return PdbE_SQL_CONDITION_EXPR_ERROR;

    if (pLeftExpr->GetOp() != TK_ID)
      return PdbE_OK;

    const std::string& fieldName = pLeftExpr->GetValueStr();

    if (StringTool::ComparyNoCase(fieldName, DEVID_FIELD_NAME, (sizeof(DEVID_FIELD_NAME) - 1)))
    {
      if (op == TK_IN || op == TK_NOTIN)
      {
        const ExprList* pArgs = pExpr->GetExprList();
        if (pArgs == nullptr)
          return PdbE_SQL_CONDITION_EXPR_ERROR;

        std::list<int64_t> argValList;
        if (!pArgs->GetIntValList(argValList))
          return PdbE_SQL_CONDITION_EXPR_ERROR;

        if (argValList.empty())
          return PdbE_SQL_CONDITION_EXPR_ERROR;

        if (op == TK_IN)
          conditionVec_.push_back(new InNumCondition(PDB_DEVID_INDEX, argValList));
        else
          conditionVec_.push_back(new NotInNumCondition(PDB_DEVID_INDEX, argValList));
      }
      else if (op == TK_ISNULL)
      {
        minDevId_ = -1;
        maxDevId_ = -1;
      }
      else if (op == TK_ISNOTNULL)
      {
        return PdbE_OK;
      }
      else
      {
        if (pRightExpr == nullptr)
          return PdbE_SQL_CONDITION_EXPR_ERROR;

        int64_t rightVal = 0;
        if (!pRightExpr->GetIntVal(&rightVal))
          return PdbE_INVALID_INT_VAL;

        switch (op)
        {
        case TK_LT:
        {
          conditionVec_.push_back(new LtNumCondition<PDB_VALUE_TYPE::VAL_INT64>(PDB_DEVID_INDEX, rightVal));
          if (maxDevId_ > rightVal) { maxDevId_ = rightVal; }
          break;
        }
        case TK_LE:
        {
          conditionVec_.push_back(new LeNumCondition<PDB_VALUE_TYPE::VAL_INT64>(PDB_DEVID_INDEX, rightVal));
          if (maxDevId_ > rightVal) { maxDevId_ = rightVal; }
          break;
        }
        case TK_GT:
        {
          conditionVec_.push_back(new GtNumCondition<PDB_VALUE_TYPE::VAL_INT64>(PDB_DEVID_INDEX, rightVal));
          if (minDevId_ < rightVal) { minDevId_ = rightVal; }
          break;
        }
        case TK_GE:
        {
          conditionVec_.push_back(new GeNumCondition<PDB_VALUE_TYPE::VAL_INT64>(PDB_DEVID_INDEX, rightVal));
          if (minDevId_ < rightVal) { minDevId_ = rightVal; }
          break;
        }
        case TK_NE:
        {
          conditionVec_.push_back(new NeNumCondition<PDB_VALUE_TYPE::VAL_INT64>(PDB_DEVID_INDEX, rightVal));
          break;
        }
        case TK_EQ:
        {
          conditionVec_.push_back(new EqNumCondition<PDB_VALUE_TYPE::VAL_INT64>(PDB_DEVID_INDEX, rightVal));
          maxDevId_ = rightVal;
          minDevId_ = rightVal;
          break;
        }
        default:
          return PdbE_SQL_CONDITION_EXPR_ERROR;
        }
      }

    }
  }

  return PdbE_OK;
}
