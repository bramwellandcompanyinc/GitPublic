using System.Collections.Generic;
using FeedMeThese.Interfaces;
using FeedMeThese.Implementations;
using FeedMeThese.DependencyInjection;

using Microsoft.VisualStudio.TestTools.UnitTesting;


namespace ProcessorTest
{
    /// <summary>
    ///This is a test class for InputParserTest and is intended
    ///to contain all InputParserTest Unit Tests
    ///</summary>
	[TestClass()]
	public class InputParserTest
	{
		private static Dictionary<string, string> testSampleData = new Dictionary<string, string>()
		{
			//The first set here is as defined in the requirements document

			{"morning, 1, 2, 3", "eggs, toast, coffee"},
			{"morning, 2, 1, 3", "eggs, toast, coffee"},
			{"morning, 1, 2, 3, 4", "eggs, toast, coffee, error"},
			{"morning, 1, 2, 3, 3, 3", "eggs, toast, coffee(x3)"},
			{"night, 1, 2, 3, 4", "steak, potato, wine, cake"},
			{"night, 1, 2, 2, 4", "steak, potato(x2), cake"},
			{"night, 1, 2, 3, 5", "steak, potato, wine, error"},
			{"night, 1, 1, 2, 3, 5", "steak, error"}
		};


		private static Dictionary<string, string> negativeSampleData = new Dictionary<string, string>()
		{
			{"", "error"},	//The behaviour in this condition is undefined in the requirements doc so made design decicion to implement this way
			{"1, 2, 1, 3", "error"},
			{"morning 1, 2, 3, 4", "error"},
			{"notmorningnotnight, 1, 2, 3, 3, 3", "error"},
			{"night, 1, 2,, 3, 4", "steak, potato, error"},
			{"night, 1, 2, 5, 2, 3", "steak, potato, error"}
		};

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
		[ClassInitialize()]
		public static void MyClassInitialize(TestContext testContext)
		{
			Dependencies.EstablishContainer();
		}

		#endregion


		/// <summary>
		///A test for Process
		///</summary>
		[TestMethod()]
		public void AcceptanceTest()
		{
			//Test the input defined in the requirements document

			IInputParser _parser = Dependencies.Resolve<IInputParser>();
			foreach (var input in testSampleData.Keys)
			{
				var output = _parser.ParseInput(input);
				Assert.AreEqual(testSampleData[input], output.ToString());
			}
		}

		/// <summary>
		///A test for Process
		///</summary>
		[TestMethod()]
		public void NegativeTests()
		{
			//Test the input defined in the requirements document

			IInputParser _parser = Dependencies.Resolve<IInputParser>();
			foreach (var input in negativeSampleData.Keys)
			{
				var output = _parser.ParseInput(input);
				Assert.AreEqual(negativeSampleData[input], output.ToString());
			}
		}
	}
}


