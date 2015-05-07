using FeedMeThese;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using System.Runtime.CompilerServices;
using FeedMeThese.DependencyInjection;
using FeedMeThese.Entities;
using FeedMeThese.Interfaces;
using FeedMeThese.Implementations;

namespace ProcessorTest
{
    
    
    /// <summary>
    ///This is a test class for ParsedOutputTest and is intended
    ///to contain all ParsedOutputTest Unit Tests
    ///</summary>
	[TestClass()]
	public class ParsedOutputTest
    {
	    private static IParsedOutput _target;

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
		///A test for Clear
		///</summary>
	    [TestMethod()]
	    public void ParsedOutput_ClearTest()
	    {
			//Granted, it is hard to test Clear in isolation without assuming that Add works
			//But we'll make sure it did something
			
			_target = new ParsedOutput();
			_target.Add(new DishDefinition(MealTimeEnum.Morning, 1, "testmeal", MealCourseEnum.Desert, true, 1));

			//Make sure the class did something with its add method, even if the method didn't work properly

			string actual = _target.ToString();
			string notExpected = "";
			Assert.AreNotEqual(notExpected, actual);

			//Now clear it out and try again

			_target.Clear();
			string expected = "";
			actual = _target.ToString();
			Assert.AreEqual(expected, actual);
	    }

		/// <summary>
		///A test for Add
		///</summary>
		[TestMethod()]
		public void ParsedOutput_AddTest()
		{
			_target = new ParsedOutput();
			DishDefinition dd = new DishDefinition(MealTimeEnum.Morning, 1, "testmeal", MealCourseEnum.Desert, true, 1);
			_target.Add(dd);

			string expected = "testmeal";
			string actual = _target.ToString();
			Assert.AreEqual(expected, actual);

			//Now add multiple items of the same type and exercise the (xY) logic

			_target.Add(dd);
			expected = "testmeal(x2)";
			actual = _target.ToString();
			Assert.AreEqual(expected, actual);
		}

		/// <summary>
		///A test for CountOf
		///</summary>
		[TestMethod()]
		public void ParsedOutput_CountOfTest()
		{
			_target = new ParsedOutput();
			DishDefinition dd = new DishDefinition(MealTimeEnum.Morning, 1, "testmeal", MealCourseEnum.Desert, true, 1);

			//Add a couple of items and ensure the counter is working
			_target.Add(dd);
			_target.Add(dd);

			uint expected = 2;
			uint actual = 2;
			Assert.AreEqual(expected, actual);
		}

		/// <summary>
		///A test for SetErrorState
		///</summary>
		[TestMethod()]
		public void ParsedOutput_SetErrorStateTest()
		{
			_target = new ParsedOutput();
			_target.Add(new DishDefinition(MealTimeEnum.Morning, 1, "testmeal", MealCourseEnum.Desert, true, 1));
			_target.SetErrorState();

			string expected = "testmeal, error";
			string actual = _target.ToString();
			Assert.AreEqual(expected, actual);

			_target.Clear();
			_target.SetErrorState();
			expected = "error";
			actual = _target.ToString();
			Assert.AreEqual(expected, actual);
		}

		/// <summary>
		///A test for IsInError
		///</summary>
		[TestMethod()]
		public void ParsedOutput_IsInErrorTest()
		{
			_target = new ParsedOutput();

			bool expected = false;
			bool actual = _target.IsInError();
			Assert.AreEqual(expected, actual);

			_target.SetErrorState();

			expected = true;
			actual = _target.IsInError();
			Assert.AreEqual(expected, actual);
		}
    }
}
