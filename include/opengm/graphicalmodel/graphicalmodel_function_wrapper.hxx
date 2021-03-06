#pragma once
#ifndef OPENGM_GRAPHICALMODEL_FUNCTION_WRAPPER_HXX
#define OPENGM_GRAPHICALMODEL_FUNCTION_WRAPPER_HXX

#include <vector>
#include <set>
#include <algorithm>
#include <functional>
#include <numeric>
#include <map>
#include <list>
#include <set>
#include <functional>

#include "opengm/functions/explicit_function.hxx"
#include "opengm/opengm.hxx"
#include "opengm/utilities/indexing.hxx"
#include "opengm/utilities/sorting.hxx"
#include "opengm/utilities/functors.hxx"
#include "opengm/utilities/metaprogramming.hxx"
#include "opengm/operations/minimizer.hxx"
#include "opengm/graphicalmodel/graphicalmodel_factor_operator.hxx"
#include "opengm/graphicalmodel/graphicalmodel_factor_accumulator.hxx"

namespace opengm {

/// \cond HIDDEN_SYMBOLS

template<
   class T,
   class OPERATOR,
   class FUNCTION_TYPE_LIST ,
   class SPACE ,
   bool MUTABLE
>
class GraphicalModel;

template<class GRAPHICAL_MODEL> class Factor;

namespace detail_graphical_model {

   #define OPENGM_BASIC_FUNCTION_WRAPPER_CODE_GENERATOR_MACRO( RETURN_TYPE , FUNCTION_NAME ) \
   template<size_t NUMBER_OF_FUNCTIONS> \
   template<class GM> \
   inline RETURN_TYPE \
   FunctionWrapper<NUMBER_OF_FUNCTIONS>::FUNCTION_NAME \
   ( \
      GM const * gm, \
      const size_t functionIndex, \
      const size_t functionType \
   ) { \
     typedef typename opengm::meta::SizeT< opengm::meta::Decrement<NUMBER_OF_FUNCTIONS>::value > MaxIndex;  \
      if(meta::EqualNumber<NUMBER_OF_FUNCTIONS,1>::value) { \
         return gm->template functions<meta::MinimumNumber<0,MaxIndex::value >::value >()[functionIndex].FUNCTION_NAME(); \
      } \
      if(meta::EqualNumber<NUMBER_OF_FUNCTIONS,2>::value) { \
         if(functionType==0) \
            return gm->template functions<meta::MinimumNumber<0,MaxIndex::value >::value >()[functionIndex].FUNCTION_NAME(); \
         else \
            return gm->template functions<meta::MinimumNumber<1,MaxIndex::value >::value >()[functionIndex].FUNCTION_NAME(); \
      } \
      if(meta::BiggerOrEqualNumber<NUMBER_OF_FUNCTIONS,3>::value) { \
         switch(functionType) { \
            case 0: \
               return gm->template functions<meta::MinimumNumber<0,MaxIndex::value >::value >()[functionIndex].FUNCTION_NAME(); \
            case 1: \
               return gm->template functions<meta::MinimumNumber<1,MaxIndex::value >::value >()[functionIndex].FUNCTION_NAME(); \
            case 2: \
               return gm->template functions<meta::MinimumNumber<2,MaxIndex::value >::value >()[functionIndex].FUNCTION_NAME(); \
            case 3: \
               return gm->template functions<meta::MinimumNumber<3,MaxIndex::value >::value >()[functionIndex].FUNCTION_NAME(); \
            case 4: \
               return gm->template functions<meta::MinimumNumber<4,MaxIndex::value >::value >()[functionIndex].FUNCTION_NAME(); \
            case 5: \
               return gm->template functions<meta::MinimumNumber<5,MaxIndex::value >::value >()[functionIndex].FUNCTION_NAME(); \
            case 6: \
               return gm->template functions<meta::MinimumNumber<6,MaxIndex::value >::value >()[functionIndex].FUNCTION_NAME(); \
            case 7: \
               return gm->template functions<meta::MinimumNumber<7,MaxIndex::value >::value >()[functionIndex].FUNCTION_NAME(); \
            case 8: \
               return gm->template functions<meta::MinimumNumber<8,MaxIndex::value >::value >()[functionIndex].FUNCTION_NAME(); \
            case 9: \
               return gm->template functions<meta::MinimumNumber<9,MaxIndex::value >::value >()[functionIndex].FUNCTION_NAME(); \
            case 10: \
               return gm->template functions<meta::MinimumNumber<10,MaxIndex::value >::value >()[functionIndex].FUNCTION_NAME(); \
            case 11: \
               return gm->template functions<meta::MinimumNumber<11,MaxIndex::value >::value >()[functionIndex].FUNCTION_NAME(); \
            case 12: \
               return gm->template functions<meta::MinimumNumber<12,MaxIndex::value >::value >()[functionIndex].FUNCTION_NAME(); \
            case 13: \
               return gm->template functions<meta::MinimumNumber<13,MaxIndex::value >::value >()[functionIndex].FUNCTION_NAME(); \
            case 14: \
               return gm->template functions<meta::MinimumNumber<14,MaxIndex::value >::value >()[functionIndex].FUNCTION_NAME(); \
            case 15: \
               return gm->template functions<meta::MinimumNumber<15,MaxIndex::value >::value >()[functionIndex].FUNCTION_NAME(); \
            default: \
               return FunctionWrapperExecutor< \
                  16, \
                  NUMBER_OF_FUNCTIONS, \
                  opengm::meta::BiggerOrEqualNumber<16,NUMBER_OF_FUNCTIONS >::value \
               >::FUNCTION_NAME(gm,functionIndex,functionType); \
         } \
      } \
   } \
   template<size_t IX, size_t DX> \
   template<class GM> \
   RETURN_TYPE FunctionWrapperExecutor<IX, DX, false>::FUNCTION_NAME \
   ( \
      GM const* gm, \
      const size_t functionIndex, \
      const size_t functionType \
   ) { \
      if(functionType==IX) { \
         return gm->template functions<IX>()[functionIndex].FUNCTION_NAME(); \
      } \
      else { \
         return FunctionWrapperExecutor<IX+1, DX, meta::Bool<IX+1==DX>::value >::FUNCTION_NAME (gm, functionIndex,functionType); \
      } \
   } \
   template<size_t IX, size_t DX> \
   template<class GM> \
   RETURN_TYPE FunctionWrapperExecutor<IX, DX, true>::FUNCTION_NAME \
   ( \
      GM const* gm, \
      const size_t functionIndex, \
      const size_t functionType \
   ) { \
      throw RuntimeError("Incorrect function type id."); \
   }

   template<size_t IX, size_t DX, bool end>
   struct FunctionWrapperExecutor;

   template<size_t IX, size_t DX>
   struct FunctionWrapperExecutor<IX,DX,false>{
      template <class GM,class ITERATOR>
      static typename GM::ValueType  getValue(const GM *,ITERATOR,const typename GM::IndexType ,const size_t );
      template <class GM>
      static void addFactorFunctionAdjacency(GM * ,const size_t ,const size_t , const size_t );
      template <class GM>
      static void swapAndDeleteFunction(GM * , const size_t , const size_t , const size_t , size_t& );
      template <class GM>
      static size_t numberOfFunctions(const GM *,const size_t );
      template <class GM>
      static void initializeFactorFunctionAdjacencies(GM *);
      template <class GM_SOURCE,class GM_DEST>
      static void assignFunctions(const GM_SOURCE & ,GM_DEST &);
      template<class GM>
      static bool isPotts(GM const *,const size_t ,const size_t);
      template<class GM>
      static bool isGeneralizedPotts(GM const *,const size_t ,const size_t);
      template<class GM>
      static bool isSubmodular(GM const *,const size_t ,const size_t);
      template<class GM>
      static typename GM::ValueType min(GM const *,const size_t ,const size_t);
      template<class GM>
      static typename GM::ValueType max(GM const *,const size_t ,const size_t);
      template<class GM>
      static typename GM::ValueType sum(GM const *,const size_t ,const size_t);
      template<class GM>
      static typename GM::ValueType product(GM const *,const size_t ,const size_t);
      template<class GM>
      static bool isSquaredDifference(GM const *,const size_t ,const size_t);
      template<class GM>
      static bool isTruncatedSquaredDifference(GM const *,const size_t ,const size_t);
      template<class GM>
      static bool isAbsoluteDifference(GM const *,const size_t ,const size_t);
      template<class GM>
      static bool isTruncatedAbsoluteDifference(GM const *,const size_t ,const size_t);
   };

   template<size_t IX, size_t DX>
   struct FunctionWrapperExecutor<IX,DX,true>{
      template <class GM,class ITERATOR>
      static typename GM::ValueType  getValue(const GM *,ITERATOR,const typename GM::IndexType ,const size_t );
      template <class GM>
      static void addFactorFunctionAdjacency(GM * ,const size_t ,const size_t , const size_t );
      template <class GM>
      static void swapAndDeleteFunction(GM * , const size_t , const size_t , const size_t , size_t& );
      template <class GM>
      static size_t numberOfFunctions(const GM *,const size_t functionTypeIndex);
      template <class GM>
      static void initializeFactorFunctionAdjacencies(GM *);
      template <class GM_SOURCE,class GM_DEST>
      static void assignFunctions(const GM_SOURCE & ,GM_DEST &);
      template<class GM>
      static bool isPotts(GM const *,const size_t ,const size_t);
      template<class GM>
      static bool isGeneralizedPotts(GM const *,const size_t ,const size_t);
      template<class GM>
      static bool isSubmodular(GM const *,const size_t,const size_t );
      template<class GM>
      static typename GM::ValueType min(GM const *,const size_t ,const size_t);
      template<class GM>
      static typename GM::ValueType max(GM const *,const size_t ,const size_t);
      template<class GM>
      static typename GM::ValueType sum(GM const *,const size_t ,const size_t);
      template<class GM>
      static typename GM::ValueType product(GM const *,const size_t ,const size_t);
      template<class GM>
      static bool isSquaredDifference(GM const *,const size_t ,const size_t);
      template<class GM>
      static bool isTruncatedSquaredDifference(GM const *,const size_t ,const size_t);
      template<class GM>
      static bool isAbsoluteDifference(GM const *,const size_t ,const size_t);
      template<class GM>
      static bool isTruncatedAbsoluteDifference(GM const *,const size_t ,const size_t);
   };

   template<size_t NUMBER_OF_FUNCTIONS>
   struct FunctionWrapper{
      template <class GM,class ITERATOR>
      static typename GM::ValueType  getValue(const GM *,ITERATOR,const typename GM::IndexType ,const size_t );
      template <class GM>
      static void addFactorFunctionAdjacency(GM * ,const size_t ,const size_t , const size_t );
      template <class GM>
      static void swapAndDeleteFunction(GM * , const size_t , const size_t , const size_t , size_t& );
      template <class GM>
      static size_t numberOfFunctions(const GM *,const size_t functionTypeIndex);
      template <class GM>
      static void initializeFactorFunctionAdjacencies(GM *);
      template <class GM_SOURCE,class GM_DEST>
      static void assignFunctions(const GM_SOURCE & ,GM_DEST &);
      template<class GM>
      static bool isPotts(GM const *,const size_t,const size_t);
      template<class GM>
      static bool isGeneralizedPotts(GM const *,const size_t ,const size_t);
      template<class GM>
      static bool isSubmodular(GM const *,const size_t ,const size_t);
      template<class GM>
      static typename GM::ValueType min(GM const *,const size_t ,const size_t);
      template<class GM>
      static typename GM::ValueType max(GM const *,const size_t ,const size_t);
      template<class GM>
      static typename GM::ValueType sum(GM const *,const size_t ,const size_t);
      template<class GM>
      static typename GM::ValueType product(GM const *,const size_t ,const size_t);
      template<class GM>
      static bool isSquaredDifference(GM const *,const size_t ,const size_t);
      template<class GM>
      static bool isTruncatedSquaredDifference(GM const *,const size_t ,const size_t);
      template<class GM>
      static bool isAbsoluteDifference(GM const *,const size_t ,const size_t);
      template<class GM>
      static bool isTruncatedAbsoluteDifference(GM const *,const size_t ,const size_t);
   };
} //namespace detail_graphical_model

// implementaion
namespace detail_graphical_model {
   OPENGM_BASIC_FUNCTION_WRAPPER_CODE_GENERATOR_MACRO( bool, isSubmodular)
   OPENGM_BASIC_FUNCTION_WRAPPER_CODE_GENERATOR_MACRO( bool, isPotts)
   OPENGM_BASIC_FUNCTION_WRAPPER_CODE_GENERATOR_MACRO( bool, isGeneralizedPotts)
   OPENGM_BASIC_FUNCTION_WRAPPER_CODE_GENERATOR_MACRO( bool, isSquaredDifference)
   OPENGM_BASIC_FUNCTION_WRAPPER_CODE_GENERATOR_MACRO( bool, isTruncatedSquaredDifference)
   OPENGM_BASIC_FUNCTION_WRAPPER_CODE_GENERATOR_MACRO( bool, isAbsoluteDifference)
   OPENGM_BASIC_FUNCTION_WRAPPER_CODE_GENERATOR_MACRO( bool, isTruncatedAbsoluteDifference)
   OPENGM_BASIC_FUNCTION_WRAPPER_CODE_GENERATOR_MACRO( typename GM::ValueType, min)
   OPENGM_BASIC_FUNCTION_WRAPPER_CODE_GENERATOR_MACRO( typename GM::ValueType, max)
   OPENGM_BASIC_FUNCTION_WRAPPER_CODE_GENERATOR_MACRO( typename GM::ValueType, sum)
   OPENGM_BASIC_FUNCTION_WRAPPER_CODE_GENERATOR_MACRO( typename GM::ValueType, product)

   template<size_t IX, size_t DX>
   template<class GM>
   void FunctionWrapperExecutor<IX, DX, false>::addFactorFunctionAdjacency
   (
      GM * gm,
      const size_t functionIndex,
      const size_t factorIndex,
      const size_t rtIndex
   ) {
      if(IX==rtIndex) {
         OPENGM_ASSERT(functionIndex< gm->template functions<IX>().size());
         OPENGM_ASSERT(functionIndex< gm->template factorFunctionAdjacencies<IX>().size());
         gm->template factorFunctionAdjacencies<IX>()[functionIndex].insert(factorIndex);
      }
      else {
         FunctionWrapperExecutor<IX+1, DX, meta::Bool<IX+1==DX>::value >::addFactorFunctionAdjacency(gm, functionIndex, factorIndex, rtIndex);
      }
   }

   template<size_t IX, size_t DX>
   template<class GM>
   void FunctionWrapperExecutor<IX, DX, true>::addFactorFunctionAdjacency
   (
      GM * gm,
      const size_t functionIndex,
      const size_t factorIndex,
      const size_t rtIndex
   ) {
      throw RuntimeError("Incorrect function type id.");
   }

   template<size_t IX, size_t DX>
   template<class GM>
   inline void FunctionWrapperExecutor<IX, DX, false>::swapAndDeleteFunction
   (
      GM * gm,
      const size_t currenFactorIndex,
      const size_t currentFunctionIndex,
      const size_t currentFunctionType,
      size_t& explicitFunctionIndex
   ) {
      typedef meta::SizeT<
         meta::Decrement<
            GM::NrOfFunctionTypes
         >::value
      > ExplicitFunctionPosition;
      if(IX==currentFunctionType) {
         bool swappedFunctionIsNewOne=false;
         if(gm->template factorFunctionAdjacencies<IX>()[currentFunctionIndex].size() == 1) {
            if(currentFunctionIndex== gm->template functions<IX>().size()-1) {
                gm->template functions<IX>().pop_back();
                gm->template factorFunctionAdjacencies<IX>().pop_back();
            }
            else{
               if(   IX==ExplicitFunctionPosition::value &&
                     explicitFunctionIndex == gm->template functions<ExplicitFunctionPosition::value>().size() - 1) {
                  swappedFunctionIsNewOne=true;
               }
               //meta::FieldAccess::template byIndex<0>(this->functionDataField_).functionData_.functions_.size();
               gm->template functions<IX>()[currentFunctionIndex] =gm->template functions<IX>().back();
               gm->template factorFunctionAdjacencies<IX>()[currentFunctionIndex] =gm->template factorFunctionAdjacencies<IX>().back();
               gm->template functions<IX>().pop_back();
               gm->template factorFunctionAdjacencies<IX>().pop_back();
               typename RandomAccessSet<typename  GM::IndexType>::const_iterator begin =gm->template factorFunctionAdjacencies<IX>()[currentFunctionIndex].begin();
               typename RandomAccessSet<typename  GM::IndexType>::const_iterator end =gm->template factorFunctionAdjacencies<IX>()[currentFunctionIndex].end();
               while(begin != end) {
                  OPENGM_ASSERT(*begin<gm->factors_.size());
                  gm->factors_[*begin].functionIndex_ = currentFunctionIndex;
                  ++begin;
               }
            }

            if(swappedFunctionIsNewOne) {
               explicitFunctionIndex = currentFunctionIndex;
            }
         }
         else {
            // remove factor index from adjaceny
            const RandomAccessSet<typename  GM::IndexType>& adset = gm->template factorFunctionAdjacencies<IX>()[currentFunctionIndex];
            typename RandomAccessSet<typename  GM::IndexType>::const_iterator begin = adset.begin();
            typename RandomAccessSet<typename  GM::IndexType>::const_iterator end = adset.end();
            gm->template factorFunctionAdjacencies<IX>()[currentFunctionIndex].erase(currenFactorIndex);
         }
      }
      else {
         typedef FunctionWrapperExecutor<IX+1, DX, meta::Bool<IX+1==DX>::value> NewExecutorType;
         NewExecutorType::swapAndDeleteFunction(gm, currenFactorIndex, currentFunctionIndex, currentFunctionType, explicitFunctionIndex);
      }
   }

   template<size_t IX, size_t DX>
   template<class GM>
   inline void FunctionWrapperExecutor<IX, DX, true>::swapAndDeleteFunction
   (
      GM * gm,
      const size_t currentFactorIndex,
      const size_t currentFunctionIndex,
      const size_t currentFunctionType,
      size_t& explicitFunctionIndex
   ) {
      throw RuntimeError("Incorrect function type id.");
   }

   template<size_t IX,size_t DX>
   template<class GM>
   inline void
   FunctionWrapperExecutor<IX,DX,false>::initializeFactorFunctionAdjacencies
   (
      GM * gm
   ) {
      const size_t nrOfFunctions=gm-> template functions<IX>().size();
      gm->template factorFunctionAdjacencies <IX>().resize(nrOfFunctions);
      typedef FunctionWrapperExecutor<
         meta::Increment<IX>::value,
         DX,
         meta::EqualNumber<meta::Increment<IX>::value,DX>::value
      > ExecutorType;
      ExecutorType::initializeFactorFunctionAdjacencies(gm);
   }

   template<size_t IX,size_t DX>
   template<class GM>
   inline void
   FunctionWrapperExecutor<IX,DX,true>::initializeFactorFunctionAdjacencies
   (
      GM * gm
   ) {
      for(size_t i=0;i<gm->numberOfFactors();++i) {
         gm->addFactorToAdjacency(gm->factors_[i].functionIndex(),i,gm->factors_[i].functionType());
      }
   }

   template<size_t IX,size_t DX>
   template<class GM,class ITERATOR>
   inline typename GM::ValueType
   FunctionWrapperExecutor<IX,DX,false>::getValue
   (
      const GM * gm,
      ITERATOR iterator,
      const typename GM::IndexType functionIndex,
      const size_t functionType
   ) {
      if(IX==functionType) {
         return gm-> template functions<IX>()[functionIndex](iterator);
      }
      else{
         return FunctionWrapperExecutor<
            meta::Increment<IX>::value,
            DX,
            meta::EqualNumber<
               meta::Increment<IX>::value,
               DX
            >::value
         >::getValue(gm,iterator,functionIndex,functionType);
      }
   }

   template<size_t IX,size_t DX>
   template<class GM,class ITERATOR>
   inline typename GM::ValueType
   FunctionWrapperExecutor<IX,DX,true>::getValue
   (
      const GM * gm,
      ITERATOR iterator,
      const typename GM::IndexType functionIndex,
      const size_t functionType
   ) {
      throw RuntimeError("Incorrect function type id.");
      return typename GM::ValueType();
   }

   template<size_t IX,size_t DX>
   template<class GM>
   inline size_t
   FunctionWrapperExecutor<IX,DX,false>::numberOfFunctions
   (
      const GM * gm,
      const size_t functionType
   ) {
      if(IX==functionType) {
         return gm->template functions<IX>().size();
      }
      else{
         return FunctionWrapperExecutor<
            meta::Increment<IX>::value,
            DX,
            meta::EqualNumber<
               meta::Increment<IX>::value,
               DX
            >::value
         >::numberOfFunctions(gm,functionType);
      }
   }

   template<size_t IX,size_t DX>
   template<class GM>
   inline size_t
   FunctionWrapperExecutor<IX,DX,true>::numberOfFunctions
   (
      const GM * gm,
      const size_t functionType
   ) {
      throw RuntimeError("Incorrect function type id.");
   }

   template<size_t IX,size_t DX>
   template<class GM_SOURCE,class GM_DEST>
   inline void
   FunctionWrapperExecutor<IX,DX,false>::assignFunctions
   (
      const GM_SOURCE & gmSource,
      GM_DEST & gmDest
   ) {
      typedef typename meta::TypeAtTypeList<
         typename GM_SOURCE::FunctionTypeList ,
         IX
      >::type SourceTypeAtIX;
      typedef meta::SizeT<
         meta::GetIndexInTypeList<
            typename GM_DEST::FunctionTypeList,
            SourceTypeAtIX
         >::value
      > PositionOfSourceTypeInDestType;
      gmDest.template functions<PositionOfSourceTypeInDestType::value> () =
         gmSource.template functions<IX> ();

      //recursive call to the rest
      FunctionWrapperExecutor<
         meta::Increment<IX>::value,
         DX,
         meta::EqualNumber<
            meta::Increment<IX>::value,
            DX
         >::value
      >::assignFunctions(gmSource,gmDest);
   }

   template<size_t IX,size_t DX>
   template<class GM_SOURCE,class GM_DEST>
   inline void
   FunctionWrapperExecutor<IX,DX,true>::assignFunctions
   (
      const GM_SOURCE & gmSource,
      GM_DEST & gmDest
   ) {
   }

   template<size_t NUMBER_OF_FUNCTIONS>
   template<class GM_SOURCE,class GM_DEST>
   inline void
   FunctionWrapper<NUMBER_OF_FUNCTIONS>::assignFunctions
   (
      const GM_SOURCE & gmSource,
      GM_DEST & gmDest
   ) {
      typedef FunctionWrapperExecutor<0, NUMBER_OF_FUNCTIONS, meta::Bool<NUMBER_OF_FUNCTIONS==0>::value> ExecutorType;
      return ExecutorType::assignFunctions(gmSource, gmDest);
   }

   template<size_t NUMBER_OF_FUNCTIONS>
   template<class GM>
   inline size_t
   FunctionWrapper<NUMBER_OF_FUNCTIONS>::numberOfFunctions
   (
      const GM * gm,
      const size_t functionType
   ) {
      typedef FunctionWrapperExecutor<0, NUMBER_OF_FUNCTIONS, meta::Bool<NUMBER_OF_FUNCTIONS==0>::value> ExecutorType;
      return ExecutorType::numberOfFunctions(gm, functionType);
   }

   template<size_t NUMBER_OF_FUNCTIONS>
   template<class GM,class ITERATOR>
   inline typename GM::ValueType
   FunctionWrapper<NUMBER_OF_FUNCTIONS>::getValue
   (
      const GM *  gm,
      ITERATOR iterator,
      const typename GM::IndexType functionIndex,
      const size_t functionType
   ) {
      typedef typename opengm::meta::SizeT< opengm::meta::Decrement<NUMBER_OF_FUNCTIONS>::value > MaxIndex;
      // special implementation if there is only one function typelist
      if(meta::EqualNumber<NUMBER_OF_FUNCTIONS,1>::value) {
         return gm->template functions<meta::MinimumNumber<0,MaxIndex::value >::value >()[functionIndex](iterator);
      }
      // special implementation if there are only two functions in the typelist
      if(meta::EqualNumber<NUMBER_OF_FUNCTIONS,2>::value) {
         if(functionType==0)
            return gm->template functions<meta::MinimumNumber<0,MaxIndex::value >::value >()[functionIndex](iterator);
         else
            return gm->template functions<meta::MinimumNumber<1,MaxIndex::value >::value >()[functionIndex](iterator);
      }
      // general case : 3 or more functions in the typelist
      if(meta::BiggerOrEqualNumber<NUMBER_OF_FUNCTIONS,3>::value) {
         switch(functionType) {
            case 0:
               return gm->template functions<meta::MinimumNumber<0,MaxIndex::value >::value >()[functionIndex](iterator);
            case 1:
               return gm->template functions<meta::MinimumNumber<1,MaxIndex::value >::value >()[functionIndex](iterator);
            case 2:
               return gm->template functions<meta::MinimumNumber<2,MaxIndex::value >::value >()[functionIndex](iterator);
            case 3:
               return gm->template functions<meta::MinimumNumber<3,MaxIndex::value >::value >()[functionIndex](iterator);
            case 4:
               return gm->template functions<meta::MinimumNumber<4,MaxIndex::value >::value >()[functionIndex](iterator);
            case 5:
               return gm->template functions<meta::MinimumNumber<5,MaxIndex::value >::value >()[functionIndex](iterator);
            case 6:
               return gm->template functions<meta::MinimumNumber<6,MaxIndex::value >::value >()[functionIndex](iterator);
            case 7:
               return gm->template functions<meta::MinimumNumber<7,MaxIndex::value >::value >()[functionIndex](iterator);
            case 8:
               return gm->template functions<meta::MinimumNumber<8,MaxIndex::value >::value >()[functionIndex](iterator);
            case 9:
               return gm->template functions<meta::MinimumNumber<9,MaxIndex::value >::value >()[functionIndex](iterator);
            case 10:
               return gm->template functions<meta::MinimumNumber<10,MaxIndex::value >::value >()[functionIndex](iterator);
            case 11:
               return gm->template functions<meta::MinimumNumber<11,MaxIndex::value >::value >()[functionIndex](iterator);
            case 12:
               return gm->template functions<meta::MinimumNumber<12,MaxIndex::value >::value >()[functionIndex](iterator);
            case 13:
               return gm->template functions<meta::MinimumNumber<13,MaxIndex::value >::value >()[functionIndex](iterator);
            case 14:
               return gm->template functions<meta::MinimumNumber<14,MaxIndex::value >::value >()[functionIndex](iterator);
            case 15:
               return gm->template functions<meta::MinimumNumber<15,MaxIndex::value >::value >()[functionIndex](iterator);
            default:
               // meta/template recursive "if-else" generation if the
               // function index is bigger than 15
               return FunctionWrapperExecutor<
                  16,
                  NUMBER_OF_FUNCTIONS,
                  opengm::meta::BiggerOrEqualNumber<16,NUMBER_OF_FUNCTIONS >::value
               >::getValue(gm,iterator,functionIndex,functionType);
         }
      }
   }

   template<size_t NUMBER_OF_FUNCTIONS>
   template<class GM>
   inline void
   FunctionWrapper<NUMBER_OF_FUNCTIONS>::initializeFactorFunctionAdjacencies
   (
      GM * gm
   ) {
      typedef FunctionWrapperExecutor<0, NUMBER_OF_FUNCTIONS, meta::Bool<NUMBER_OF_FUNCTIONS==0>::value> ExecutorType;
      ExecutorType::initializeFactorFunctionAdjacencies(gm);
   }

   template<size_t NUMBER_OF_FUNCTIONS>
   template<class GM>
   inline void FunctionWrapper<NUMBER_OF_FUNCTIONS>::addFactorFunctionAdjacency
   (
      GM * gm,
      const size_t functionIndex,
      const size_t factorIndex,
      const size_t rtIndex
   ) {
      typedef FunctionWrapperExecutor<0, NUMBER_OF_FUNCTIONS, meta::Bool<NUMBER_OF_FUNCTIONS==0>::value> ExecutorType;
      ExecutorType::addFactorFunctionAdjacency(gm, functionIndex, factorIndex, rtIndex);
   }

   template<size_t NUMBER_OF_FUNCTIONS>
   template<class GM>
   inline void FunctionWrapper<NUMBER_OF_FUNCTIONS>::swapAndDeleteFunction
   (
      GM * gm,
      const size_t currenFactorIndex,
      const size_t currentFunctionIndex,
      const size_t currentFunctionType,
      size_t& explicitFunctionIndex
   ) {
      typedef FunctionWrapperExecutor<0, NUMBER_OF_FUNCTIONS, meta::Bool<NUMBER_OF_FUNCTIONS==0>::value> ExecutorType;
      ExecutorType::swapAndDeleteFunction(gm, currenFactorIndex, currentFunctionIndex, currentFunctionType, explicitFunctionIndex);
   }

} // namespace detail_graphical_model

/// \endcond

} // namespace opengm

#endif // #ifndef OPENGM_GRAPHICALMODEL_FUNCTION_WRAPPER_HXX
