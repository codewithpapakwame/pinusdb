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

#include "query/result_filter_group_tstamp.h"
#include "expr/parse.h"
#include "expr/group_opt.h"
#include "query/agg_avg_function.h"
#include "query/agg_count_function.h"
#include "query/agg_last_function.h"
#include "query/agg_max_function.h"
#include "query/agg_min_function.h"
#include "query/agg_sum_function.h"
#include "query/agg_tstamp.h"
#include "util/string_tool.h"
#include "query/tstamp_filter.h"

ResultFilterGroupTStamp::ResultFilterGroupTStamp()
{
  this->timeGroupRange_ = 0;
  this->minTimeStamp_ = 0;
  this->maxTimeStamp_ = 0;

  this->lastGroupId_ = 0;
  this->pLastObj_ = nullptr;
}

ResultFilterGroupTStamp::~ResultFilterGroupTStamp()
{
}

PdbErr_t ResultFilterGroupTStamp::InitGrpTsResult()
{
  ResultObject* pRstObj = nullptr;
  int64_t grpCnt = (maxTimeStamp_ - minTimeStamp_ + timeGroupRange_ - 1) / timeGroupRange_;
  for (int64_t grpId = 0; grpId < grpCnt; grpId++)
  {
    pRstObj = new ResultObject(fieldVec_, 0, (minTimeStamp_ + (grpId * timeGroupRange_)));
    if (pRstObj == nullptr)
      return PdbE_NOMEM;

    objVec_.push_back(pRstObj);
    objMap_.insert(std::pair<uint64_t, ResultObject*>((uint64_t)grpId, pRstObj));
  }

  return PdbE_OK;
}

PdbErr_t ResultFilterGroupTStamp::AppendData(const DBVal* pVals, size_t valCnt, bool* pIsAdded)
{
  bool resultVal = true;
  PdbErr_t retVal = PdbE_OK;

  int64_t tstamp = DBVAL_ELE_GET_DATETIME(pVals, PDB_TSTAMP_INDEX);

  uint64_t groupId = (tstamp - minTimeStamp_) / timeGroupRange_;
  ResultObject* pRstObj = nullptr;

  if (pIsAdded != nullptr)
    *pIsAdded = false;

  if (tstamp >= maxTimeStamp_)
    return PdbE_RESLT_FULL; //�Ѿ�����������ʱ���

  if (tstamp < minTimeStamp_)
    return PdbE_OK;

  if (lastGroupId_ == groupId && pLastObj_ != nullptr)
  {
    pRstObj = pLastObj_;
  }
  else
  {
    auto objIter = objMap_.find(groupId);
    if (objIter == objMap_.end())
      return PdbE_OK;

    lastGroupId_ = groupId;
    pLastObj_ = objIter->second;
    pRstObj = objIter->second;
  }

  retVal = this->condiFilter_.RunCondition(pVals, valCnt, resultVal);
  if (retVal != PdbE_OK)
    return retVal;

  if (resultVal)
  {
    retVal = pRstObj->AppendData(pVals, valCnt);
    if (pIsAdded != nullptr)
    {
      *pIsAdded = (retVal == PdbE_OK);
    }
  }

  return PdbE_OK;
}

PdbErr_t ResultFilterGroupTStamp::BuildCustomFilter(const QueryParam* pQueryParam,
  const TableInfo* pTabInfo, Arena* pArena)
{
  PdbErr_t retVal = PdbE_OK;

  const ExprItem* pConditionItem = pQueryParam->pWhere_;
  const ExprList* pColList = pQueryParam->pSelList_;
  const GroupOpt* pGroupOpt = pQueryParam->pGroup_;

  //��Ҫ��ʼ��������Ϣ
  if (pGroupOpt == nullptr)
    return PdbE_INVALID_PARAM;

  retVal = pGroupOpt->GetTStampStep(timeGroupRange_);
  if (retVal != PdbE_OK)
    return retVal;

  TStampFilter timeStampFilter;
  if (!timeStampFilter.BuildFilter(pConditionItem))
  {
    return PdbE_SQL_ERROR;
  }

  minTimeStamp_ = timeStampFilter.GetMinTstamp();
  if (minTimeStamp_ > 0)
  {
    minTimeStamp_ += (queryOffset_ * timeGroupRange_);
    maxTimeStamp_ = minTimeStamp_ + (timeGroupRange_ * queryRecord_);
  }
  else
  {
    return PdbE_SQL_GROUP_LOST_BEGIN_TSTAMP;
  }

  if (timeStampFilter.GetMaxTstamp() < maxTimeStamp_)
  {
    maxTimeStamp_ = timeStampFilter.GetMaxTstamp();
  }

  if (maxTimeStamp_ < minTimeStamp_)
  {
    isEmptySet_ = true;
  }

  retVal = BuildGroupResultField(pColList->GetExprList(), pTabInfo, pGroupOpt, pArena);
  if (retVal != PdbE_OK)
    return retVal;

  return retVal;
}
