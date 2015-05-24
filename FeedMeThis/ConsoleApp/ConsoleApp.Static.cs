using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;

//The following could be configured using Unity without being included here explicitly
//but in the interests of time I'm going to include them with early binding, although
//the invokations of interfaces is late bound.

using FeedMeThis.RequestParsers;
using FeedMeThis.OutputFormatters;
using FeedMeThis.Rules;

using FeedMeThis.Framework;
using FeedMeThis.Interfaces.Client;

namespace FeedMeThis
{
	partial class ConsoleApp
	{
		public static int Main(string[] args)
		{
			//Register all concrete implementations using the factory.
			//In the real world this would be a configuration-driven activity using Unity configuration, or similar.

			bool blnSuccess = false;

			try
			{
				//On the face of it this might be nicer to do during construction of the class but throwing exceptions inside a constructor
				//isn't always considered a nice idea, and it is quite possible that exceptions could be thrown during the initialisation process.

				//Register the concrete types the console app knows about in order to service the interfaces

				Factory.RegisterType<IDishTypeInputParser, TextInputDishRequestParser>();
				Factory.RegisterType<IOutputFormatter, TextOutputFormatter>();
				Factory.RegisterType<IOrderRuleEngine, OrderRuleEngine>();

				blnSuccess = true;
			}
			catch (Exception ex)
			{
				Console.WriteLine("An error has occurred during initialisation of the application's framework.  The application will terminate:");
				Console.WriteLine(ex.ToString());
			}

			if (blnSuccess)
			{
				ConsoleApp app = new ConsoleApp();
				blnSuccess = app.Execute(args);
			}

			return blnSuccess ? 0:-1;
		}
	}
}
