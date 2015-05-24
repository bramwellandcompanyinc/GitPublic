using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.IO;
using System.Text;

using FeedMeThis.Framework;
using FeedMeThis.Interfaces.Client;
using FeedMeThis.RequestParsers;  //!!!!!!!!!!! INJECT
using FeedMeThis.OutputFormatters;  //!!!!!!!!!!! INJECT
using FeedMeThis.Rules; //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!INJECT

namespace FeedMeThis
{
	//The instance portions of this class deal with input gathering and output rendering

	partial class ConsoleApp
	{
		private bool Execute(string[] args)
		{
			bool blnSuccess = false; //Used to flag progress throughout parsing, and drives the module return value

			try
			{
				List<IDishRequest> drRequests = null; //Will hold the list of parsed dish type instances
				string strValidationFaultMessage = ""; //Will hold an error message if the input could not be parsed into meaningful items
				ConsoleApp app = new ConsoleApp();

				blnSuccess = app.GatherItemInput(args, out drRequests, out strValidationFaultMessage);
				if (blnSuccess)
				{
					IOrderRuleEngine engine = Factory.Instantiate<IOrderRuleEngine>(); //OrderRuleEngine
					List<IDishDisplay> dtProcessedRequests = engine.Process(drRequests);

					Debug.Assert(dtProcessedRequests.Count > 0); //By rule engine definition this should never be zero but lets find out early

					app.ProduceItemOutput(dtProcessedRequests);
				}
				else
					Console.WriteLine(strValidationFaultMessage);
			}
			catch (Exception ex)
			{
				Console.WriteLine("An error has occurred that cannot be handled by the normal flow of code and the application will terminate:");
				Console.WriteLine(ex.ToString());
			}

			return blnSuccess;
		}


		//Capture a comma delimited string of integers, with, however, the first item being either of "morning" or "night"

		private bool GatherItemInput(string[] args, out List<IDishRequest> requests, out string strValidationFaultMessage)
		{
			
			bool blnSuccess = false;
			IDishTypeInputParser parser = null;
			object objInput = null;


			if (CommandLineParmsProvided(args))
			{
				//If input was provided on the command line, use it instead of gathering input from the console.
				//This would aid in automated integration testing and is included only as a thought about how this could
				//be implemented.  The text input parser could actually borrow logic (derive from) the array input parser.
				//This will throw an exception if command line arguments are provided because this class is intentionally not implemented.

				parser = new ArrayInputDishRequestParser();
				objInput = args;
			}
			else
			{
				//Capture the input string from the console

				parser = Factory.Instantiate<IDishTypeInputParser>(); // new TextInputDishRequestParser();
				Console.Write("Input: ");
				string strMenuRequest = Console.In.ReadLine();
				objInput = strMenuRequest;
			}

			//Validate the input string, returning a list of IDishType

			blnSuccess = parser.Parse(objInput, out requests, out strValidationFaultMessage);

			return blnSuccess;
		}


		//Invoke the formatter to cause the dish items to render themselves

		private void ProduceItemOutput(List<IDishDisplay> requests)
		{
			IOutputFormatter of = Factory.Instantiate<IOutputFormatter>(); // new TextOutputFormatter();
			TextWriter tw = of.FormatOutput(requests);

			Console.Write("Output: ");
			Console.WriteLine(tw);
		}


		//Test for the presence of command line parameters

		private bool CommandLineParmsProvided(string[] args)
		{
			return args != null && args.Length > 0;
		}
	}
}
