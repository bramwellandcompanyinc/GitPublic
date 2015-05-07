using FeedMeThese.Entities;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using FeedMeThese.Interfaces;

namespace ProcessorTest
{
    
    
    /// <summary>
    ///This is a test class for DishDisplayTest and is intended
    ///to contain all DishDisplayTest Unit Tests
    ///</summary>
	[TestClass()]
	public class DishDisplayTest
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
		///A test for Count
		///</summary>
		[TestMethod()]
		public void DishDisplay_CountTest()
		{
			DishDefinition d1 = new DishDefinition(MealTimeEnum.Morning, 1, "meal1", MealCourseEnum.Entre, false, 1);
			DishDisplay target = new DishDisplay(1, false, d1); //We're setting up the display item such that it already has one menu item

			//Firstly make sure the count prints as expected

			uint expected = 1;
			uint actual;
			target.Count = expected;
			actual = target.Count;
			Assert.AreEqual(expected, actual);

			//Now increment the count and try again

			target.Count++;
			expected = 2;
			actual = target.Count;
			Assert.AreEqual(expected, actual);
		}


		/// <summary>
		///A test for ToString
		///</summary>
		[TestMethod()]
		public void DishDisplay_ToStringTest()
		{
			DishDefinition d1 = new DishDefinition(MealTimeEnum.Morning, 1, "meal1", MealCourseEnum.Entre, false, 1);
			DishDisplay target = new DishDisplay(1, false, d1); //We're setting up the display item such that it already has one menu item

			string actual;
			string expected = "meal1";
			actual = target.ToString();
			Assert.AreEqual(expected, actual);

			//Now increment the count a couple of times and make sure that display works

			expected = "meal1(x3)";
			target.Count += 2;
			actual = target.ToString();
			Assert.AreEqual(expected, actual);
		}

	}
}
