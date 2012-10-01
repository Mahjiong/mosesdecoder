#ifndef moses_WordTranslationFeature_h
#define moses_WordTranslationFeature_h

#include <string>
#include <map>

#include "FeatureFunction.h"
#include "FactorCollection.h"

#include "Sentence.h"
#include "FFState.h"

#ifdef WITH_THREADS
#include <boost/thread/tss.hpp>
#endif

namespace Moses
{

/** Sets the features for word translation
 */
class WordTranslationFeature : public StatelessFeatureFunction {

  typedef std::map< char, short > CharHash;
  typedef std::vector< std::set<std::string> > DocumentVector;
	
  struct ThreadLocalStorage
  {
    const Sentence *input;
    long docid;
    long topicid;
    bool use_topicid;
    const std::vector<std::string> *topicid_prob;
    bool use_topicid_prob;
  };


private:
#ifdef WITH_THREADS
  boost::thread_specific_ptr<ThreadLocalStorage> m_local;
#else
  std::auto_ptr<ThreadLocalStorage> m_local;
#endif

  std::set<std::string> m_vocabSource;
  std::set<std::string> m_vocabTarget;
  DocumentVector m_vocabDomain;
  FactorType m_factorTypeSource;
  FactorType m_factorTypeTarget;
  bool m_unrestricted;
  bool m_simple;
  bool m_sourceContext;
  bool m_targetContext;
  bool m_domainTrigger;
  float m_sparseProducerWeight;
  bool m_ignorePunctuation;
  CharHash m_punctuationHash;
  
public:
	WordTranslationFeature(FactorType factorTypeSource, FactorType factorTypeTarget,
			       bool simple, bool sourceContext, bool targetContext, bool ignorePunctuation, bool domainTrigger):
     StatelessFeatureFunction("wt", ScoreProducer::unlimited),
     m_factorTypeSource(factorTypeSource),
     m_factorTypeTarget(factorTypeTarget),
     m_unrestricted(true),
     m_simple(simple),
     m_sourceContext(sourceContext),
     m_targetContext(targetContext),
     m_domainTrigger(domainTrigger),
     m_sparseProducerWeight(1),
     m_ignorePunctuation(ignorePunctuation)
  {
		std::cerr << "Initializing word translation feature.. ";
		if (m_simple == 1) std::cerr << "using simple word translations.. ";
		if (m_sourceContext == 1) std::cerr << "using source context.. ";
		if (m_targetContext == 1) std::cerr << "using target context.. ";
		if (m_domainTrigger == 1) std::cerr << "using domain triggers.. ";
		
		  // compile a list of punctuation characters
		  if (m_ignorePunctuation) {
			  std::cerr << "ignoring punctuation for triggers.. ";
			  char punctuation[] = "\"'!?¿·()#_,.:;•&@‑/\\0123456789~=";
			  for (size_t i=0; i < sizeof(punctuation)-1; ++i)
				  m_punctuationHash[punctuation[i]] = 1;
		  }
		
		std::cerr << "done." << std::endl;
  }
      
	bool Load(const std::string &filePathSource, const std::string &filePathTarget);

	void InitializeForInput( Sentence const& in );

//  void Evaluate(const TargetPhrase& cur_phrase, ScoreComponentCollection* accumulator) const;

  const FFState* EmptyHypothesisState(const InputType &) const {
  	return new DummyState();
  }

  void Evaluate(const Hypothesis& cur_hypo,
  		          ScoreComponentCollection* accumulator) const;

  void EvaluateChart(const ChartHypothesis& cur_hypo,
  								   int featureID,
                     ScoreComponentCollection* accumulator) const;

  // basic properties
	std::string GetScoreProducerWeightShortName(unsigned) const { return "wt"; }
	size_t GetNumInputScores() const { return 0; }
	
	void SetSparseProducerWeight(float weight) { m_sparseProducerWeight = weight; }
	float GetSparseProducerWeight() const { return m_sparseProducerWeight; }
};

}

#endif // moses_WordTranslationFeature_h
