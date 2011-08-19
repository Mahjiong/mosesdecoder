#pragma once
// $Id$
/***********************************************************************
 Moses - factored phrase-based, hierarchical and syntactic language decoder
 Copyright (C) 2009 Hieu Hoang

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 ***********************************************************************/
#include <string>
#include <fstream>
#include "Vocab.h"
#include "PhraseNode.h"
#include "../../moses/src/Word.h"

namespace OnDiskPt
{
const float DEFAULT_COUNT = 66666;

class OnDiskWrapper
{
protected:
  Vocab m_vocab;
  std::string m_filePath;
  int m_numSourceFactors, m_numTargetFactors, m_numScores;
  std::fstream m_fileMisc, m_fileVocab, m_fileSource, m_fileTarget, m_fileTargetInd, m_fileTargetColl;

  size_t m_defaultNodeSize;
  PhraseNode *m_rootSourceNode;

  std::map<std::string, UINT64> m_miscInfo;

  void SaveMisc();
  bool OpenForLoad(const std::string &filePath);
  bool LoadMisc();

public:
  OnDiskWrapper();
  ~OnDiskWrapper();

  bool BeginLoad(const std::string &filePath);

  bool BeginSave(const std::string &filePath
                 , int numSourceFactors, int	numTargetFactors, int numScores);
  void EndSave();

  Vocab &GetVocab() {
    return m_vocab;
  }

  size_t GetSourceWordSize() const;
  size_t GetTargetWordSize() const;

  std::fstream &GetFileSource() {
    return m_fileSource;
  }
  std::fstream &GetFileTargetInd() {
    return m_fileTargetInd;
  }
  std::fstream &GetFileTargetColl() {
    return m_fileTargetColl;
  }
  std::fstream &GetFileVocab() {
    return m_fileVocab;
  }

  size_t GetNumSourceFactors() const {
    return m_numSourceFactors;
  }
  size_t GetNumTargetFactors() const {
    return m_numTargetFactors;
  }

  size_t GetNumScores() const {
    return m_numScores;
  }
  size_t GetNumCounts() const {
    return 1;
  }

  PhraseNode &GetRootSourceNode();

  UINT64 GetMisc(const std::string &key) const;

  Word *ConvertFromMoses(Moses::FactorDirection direction
                         , const std::vector<Moses::FactorType> &factorsVec
                         , const Moses::Word &origWord) const;

};

}
