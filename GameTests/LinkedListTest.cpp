#include "stdafx.h"
#include "../mokomes/linkedList.h"

using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;
using namespace Microsoft::VisualStudio::TestTools::UnitTesting;

namespace GameTests
{
	[TestClass]
	public ref class LinkedListTest
	{
	private:
		TestContext^ testContextInstance;

	public: 
		/// <summary>
		///Gets or sets the test context which provides
		///information about and functionality for the current test run.
		///</summary>
		property Microsoft::VisualStudio::TestTools::UnitTesting::TestContext^ TestContext
		{
			Microsoft::VisualStudio::TestTools::UnitTesting::TestContext^ get()
			{
				return testContextInstance;
			}
			System::Void set(Microsoft::VisualStudio::TestTools::UnitTesting::TestContext^ value)
			{
				testContextInstance = value;
			}
		};

		#pragma region Additional test attributes
		//
		//You can use the following additional attributes as you write your tests:
		//
		//Use ClassInitialize to run code before running the first test in the class
		//[ClassInitialize()]
		//static void MyClassInitialize(TestContext^ testContext) {};
		//
		//Use ClassCleanup to run code after all tests in a class have run
		//[ClassCleanup()]
		//static void MyClassCleanup() {};
		//
		//Use TestInitialize to run code before running each test
		//[TestInitialize()]
		//void MyTestInitialize() {};
		//
		//Use TestCleanup to run code after each test has run
		//[TestCleanup()]
		//void MyTestCleanup() {};
		//
		#pragma endregion 

		[TestMethod]
		void ListoSkaitliukoTestas()
		{
			MyLinkedList<int> *testList = new MyLinkedList<int>();
			    
			Assert::AreEqual(0, testList->count(), "");
			testList->add(new int (100), true);

			Assert::AreEqual(1, testList->count(), "");
			testList->add(new int (101), true);

			Assert::AreEqual(2, testList->count(), "");
			testList->add(new int (102), true);

			Assert::AreEqual(3, testList->count(), "");

			testList->paste(testList->cut(testList->get(0)));

			Assert::AreEqual(3, testList->count(), "");
			
			//Assert::AreEqual(101, *testList->get(1), "");
			
			//Assert::AreEqual(0, testList->count(), "");

			delete testList;
		};

		[TestMethod]
		void SkaitymoTestas()
		{
			MyLinkedList<int> *testList = new MyLinkedList<int>();
			    
			// idedam 3 elementus
			testList->add(new int (100), true);
			testList->add(new int (101), true);
			testList->add(new int (102), true);

			Assert::AreEqual(100, *testList->get(0), "");
			Assert::AreEqual(101, *testList->get(1), "");
			Assert::AreEqual(102, *testList->get(2), "");

			// viena is vidurio istrinam
			testList->remove(testList->get(1));
			Assert::AreEqual(102, *testList->get(1), "");

			delete testList;
		};


		[TestMethod]
		void IskirpimoIklijavimoTestas()
		{
			MyLinkedList<int> *testList = new MyLinkedList<int>();
			    
			// idedam 3 elementus
			testList->add(new int (100), true);
			testList->add(new int (101), true);
			testList->add(new int (102), true);


			// pirma iskerpam ir iklijuojam atgal i lista. Iklijuoja pabaigoje!
			testList->paste(testList->cut(testList->get(0)));



			Assert::AreEqual(101, *testList->get(0), "");
			Assert::AreEqual(102, *testList->get(1), "");
			Assert::AreEqual(100, *testList->get(2), "");

			

			delete testList;
		};


		[TestMethod]
		void ElementuIstrynimoTestas()
		{
			MyLinkedList<int> *testList = new MyLinkedList<int>();
			    
			// idedam 3 elementus
			testList->add(new int (100), true);
			testList->add(new int (101), true);
			testList->add(new int (102), true);


			
			testList->remove(testList->get(1));



			Assert::AreEqual(100, *testList->get(0), "");			
			Assert::AreEqual(102, *testList->get(1), "");

			delete testList;
		};

	};
}
