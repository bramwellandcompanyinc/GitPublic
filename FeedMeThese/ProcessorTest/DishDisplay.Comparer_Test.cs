using FeedMeThese.Entities;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using FeedMeThese.Interfaces;

namespace ProcessorTest
{
    
    
    /// <summary>
    ///This is a test class for DishDisplay_DishDisplayComparerTest and is intended
    ///to contain all DishDisplay_DishDisplayComparerTest Unit Tests
    ///</summary>
	[TestClass()]
	public class DishDisplay_DishDisplayComparerTest
	{


		private TestContext testContextInstance;

		/// <summary>
		///Gets or sets the test context which provides
		///information about and functionality for the current test run.
		///</summary>
		public TestContext TestContext
		{
			get
			{
				return testContextInstance;
			}
			set
			{
				testContextInstance = value;
			}
		}

		#region Additional test attributes
		// 
		//You can use the following additional attributes as you write your tests:
		//
		//Use ClassInitialize to run code before running the first test in the class
		//[ClassInitialize()]
		//public static void MyClassInitialize(TestContext testContext)
		//{
		//}
		//
		//Use ClassCleanup to run code after all tests in a class have run
		//[ClassCleanup()]
		//public static void MyClassCleanup()
		//{
		//}
		//
		//Use TestInitialize to run code before running each test
		//[TestInitialize()]
		//public void MyTestInitialize()
		//{
		//}
		//
		//Use TestCleanup to run code after each test has run
		//[TestCleanup()]
		//public void MyTestCleanup()
		//{
		//}
		//
		#endregion


		/// <summary>
		///A test for Compare
		///</summary>
		[TestMethod()]
		[DeploymentItem("FeedMeThese.exe")]
		public void DishDisplay_DishDisplayComparer_CompareTest()
		{
			DishDefinition d1 = new DishDefinition(MealTimeEnum.Morning, 1, "meal1", MealCourseEnum.Entre, false, 1);
			DishDefinition d2 = new DishDefinition(MealTimeEnum.Morning, 2, "meal2", MealCourseEnum.Entre, false, 1);
			DishDefinition d3 = new DishDefinition(MealTimeEnum.Morning, 1, "meal1", MealCourseEnum.Entre, false, 1);
			
			IDishDisplay x = new DishDisplay(1, false, d1);
			IDishDisplay y = new DishDisplay(2, false, d2);
			IDishDisplay z = new DishDisplay(1, false, d1);
			IDishDisplay n = new DishDisplay(3, false, null);

			DishDisplay_Accessor.DishDisplayComparer target = new DishDisplay_Accessor.DishDisplayComparer();

			//First test d1 > d2

			int expected = -1; // d1 is actually less than d2 because the index for the same meal type is smaller
			int actual;
			actual = target.Compare(x, y);
			Assert.AreEqual(expected, actual);
			
			//Now test d1 < d2

			expected = 1; // d1 is actually less than d2 because the index for the same meal type is smaller
			actual = target.Compare(y, x);
			Assert.AreEqual(expected, actual);
			

			//Now test d1 == d3

			expected = 0;
			actual = target.Compare(x, z);
			Assert.AreEqual(expected, actual);


			//Now test d1 > null (null is always highest)

			expected = -1;
			actual = target.Compare(x, n);
			Assert.AreEqual(expected, actual);


			//Now test d1 < null

			expected = 1;
			actual = target.Compare(n, x);
			Assert.AreEqual(expected, actual);
		}
	}
}
