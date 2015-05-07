using System;
using System.Collections.Generic;
using FeedMeThese.Implementations;
using FeedMeThese.Interfaces;
using FeedMeThese.DependencyInjection;

namespace FeedMeThese
{
	class Program
	{
		private static void Main(string[] args)
		{
			Dependencies.EstablishContainer();
			IInputParser ip = Dependencies.Resolve<IInputParser>();

			while (true)
			{
				Console.Write("Input: ");
				string input = Console.In.ReadLine();

				if (input.ToLower().Trim() == "quit")
					return;

				IParsedOutput output = ip.ParseInput(input);

				Console.WriteLine("Output: " + output + "\n");
			}
		}
	}
}
