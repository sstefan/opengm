
#include <stdlib.h>
#include <vector>
#include <set>
#include <functional>

#include <opengm/graphicalmodel/graphicalmodel.hxx>
#include <opengm/operations/adder.hxx>
#include <opengm/operations/multiplier.hxx>
#include <opengm/operations/minimizer.hxx>
#include <opengm/operations/maximizer.hxx>
#include <opengm/inference/graphcut.hxx>

#include <opengm/unittests/blackboxtester.hxx>
#include <opengm/unittests/blackboxtests/blackboxtestgrid.hxx>
#include <opengm/unittests/blackboxtests/blackboxtestfull.hxx>
#include <opengm/unittests/blackboxtests/blackboxteststar.hxx>

#ifdef WITH_BOOST
#  include <opengm/inference/auxiliary/minstcutboost.hxx>
#endif
#ifdef WITH_MAXFLOW
#  include <opengm/inference/auxiliary/minstcutkolmogorov.hxx>
#endif

int main() {
   typedef opengm::GraphicalModel<float, opengm::Adder> GraphicalModelType;
   typedef opengm::BlackBoxTestGrid<GraphicalModelType> GridTest;
   typedef opengm::BlackBoxTestFull<GraphicalModelType> FullTest;
   typedef opengm::BlackBoxTestStar<GraphicalModelType> StarTest;

   opengm::InferenceBlackBoxTester<GraphicalModelType> minTester;
   minTester.addTest(new GridTest(4, 4, 2, false, true, GridTest::POTTS, opengm::OPTIMAL, 1));
   minTester.addTest(new GridTest(3, 3, 2, false, true, GridTest::POTTS, opengm::OPTIMAL, 3));
   minTester.addTest(new GridTest(3, 3, 2, false, false,GridTest::POTTS, opengm::OPTIMAL, 3));
   minTester.addTest(new StarTest(5,    2, false, true, StarTest::POTTS, opengm::OPTIMAL, 3));
   minTester.addTest(new FullTest(5,    2, false, 3,    FullTest::POTTS, opengm::OPTIMAL, 3));
  
   opengm::InferenceBlackBoxTester<GraphicalModelType> maxTester;
   maxTester.addTest(new GridTest(4, 4, 2, false, true, GridTest::IPOTTS, opengm::OPTIMAL, 1));
   maxTester.addTest(new GridTest(3, 3, 2, false, true, GridTest::IPOTTS, opengm::OPTIMAL, 3));
   maxTester.addTest(new GridTest(3, 3, 2, false, false,GridTest::IPOTTS, opengm::OPTIMAL, 3));
   maxTester.addTest(new StarTest(5,    2, false, true, StarTest::IPOTTS, opengm::OPTIMAL, 3));
   maxTester.addTest(new FullTest(5,    2, false, 3,    FullTest::IPOTTS, opengm::OPTIMAL, 3));

   std::cout << "Test Graphcut ..." << std::endl;

#ifdef WITH_MAXFLOW
   std::cout << "  * Test Min-Sum with Kolmogorov" << std::endl;
   {
      typedef opengm::external::MinSTCutKolmogorov<size_t, float> MinStCutType;
      typedef opengm::GraphCut<GraphicalModelType, opengm::Minimizer, MinStCutType> MinGraphCut;
      MinGraphCut::Parameter para;
      minTester.test<MinGraphCut>(para);
   }
#endif

#ifdef WITH_BOOST
   std::cout << "  * Test Min-Sum with BOOST-Push-Relabel" << std::endl;
   {
      typedef opengm::MinSTCutBoost<size_t, float, opengm::PUSH_RELABEL> MinStCutType;
      typedef opengm::GraphCut<GraphicalModelType, opengm::Minimizer, MinStCutType> MinGraphCut;
      MinGraphCut::Parameter para;
      minTester.test<MinGraphCut>(para);
   } 
   std::cout << "  * Test Min-Sum with Interegr-BOOST-Push-Relabel" << std::endl;
   {
      typedef opengm::MinSTCutBoost<size_t, long, opengm::PUSH_RELABEL> MinStCutType;
      typedef opengm::GraphCut<GraphicalModelType, opengm::Minimizer, MinStCutType> MinGraphCut;
      MinGraphCut::Parameter para(1000000);
      minTester.test<MinGraphCut>(para);
   }
   std::cout << "  * Test Min-Sum with BOOST-Edmonds-Karp" << std::endl;
   {
      typedef opengm::MinSTCutBoost<size_t, float, opengm::EDMONDS_KARP> MinStCutType;
      typedef opengm::GraphCut<GraphicalModelType, opengm::Minimizer, MinStCutType> MinGraphCut;
      MinGraphCut::Parameter para;
      minTester.test<MinGraphCut>(para);
   }
   std::cout << "  * Test Min-Sum with BOOST-Kolmogorov" << std::endl;
   {
      typedef opengm::MinSTCutBoost<size_t, float, opengm::KOLMOGOROV> MinStCutType;
      typedef opengm::GraphCut<GraphicalModelType, opengm::Minimizer, MinStCutType> MinGraphCut;
      MinGraphCut::Parameter para;
      minTester.test<MinGraphCut>(para);
   }
#endif

#ifdef WITH_MAXFLOW
   std::cout << "  * Test Max-Sum with Kolmogorov" << std::endl;
   {
      typedef opengm::external::MinSTCutKolmogorov<size_t, float> MinStCutType;
      typedef opengm::GraphCut<GraphicalModelType, opengm::Maximizer, MinStCutType> MaxGraphCut;
      MaxGraphCut::Parameter para;
      maxTester.test<MaxGraphCut>(para);

   }
#endif

#ifdef WITH_BOOST
   std::cout << "  * Test Max-Sum with BOOST-Push-Relabel" << std::endl;
   {
      typedef opengm::MinSTCutBoost<size_t, float, opengm::PUSH_RELABEL> MinStCutType;
      typedef opengm::GraphCut<GraphicalModelType, opengm::Maximizer, MinStCutType> MaxGraphCut;
      MaxGraphCut::Parameter para;
      maxTester.test<MaxGraphCut>(para);
   }
   std::cout << "  * Test Max-Sum with Integer-BOOST-Push-Relabel" << std::endl;
   {
      typedef opengm::MinSTCutBoost<size_t, int, opengm::PUSH_RELABEL> MinStCutType;
      typedef opengm::GraphCut<GraphicalModelType, opengm::Maximizer, MinStCutType> MaxGraphCut;
      MaxGraphCut::Parameter para(0.000000);
      maxTester.test<MaxGraphCut>(para);
   }
   std::cout << "  * Test Max-Sum with BOOST-Edmonds-Karp" << std::endl;
   {
      typedef opengm::MinSTCutBoost<size_t, float, opengm::EDMONDS_KARP> MinStCutType;
      typedef opengm::GraphCut<GraphicalModelType, opengm::Maximizer, MinStCutType> MaxGraphCut;
      MaxGraphCut::Parameter para;
      maxTester.test<MaxGraphCut>(para);
   }
   std::cout << "  * Test Max-Sum with BOOST-Kolmogorov" << std::endl;
   {
      typedef opengm::MinSTCutBoost<size_t, float, opengm::KOLMOGOROV> MinStCutType;
      typedef opengm::GraphCut<GraphicalModelType, opengm::Maximizer, MinStCutType> MaxGraphCut;
      MaxGraphCut::Parameter para;
      maxTester.test<MaxGraphCut>(para);
   }
#endif

   return 0;
}
