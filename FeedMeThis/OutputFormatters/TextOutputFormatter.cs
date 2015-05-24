using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

using FeedMeThis.Interfaces.Client;

namespace FeedMeThis.OutputFormatters
{
	public class TextOutputFormatter : IOutputFormatter
	{
		//Create a comma delimited output writer of the processed dish items.
		//Upon encountering an error entry the output will display "error" and stop
		//processing further entries.

		public TextWriter FormatOutput(List<IDishDisplay> items)
		{
			TextWriter tw = new StringWriter();
			bool blnContinue = true;

			foreach (IDishDisplay dd in items)
			{
				if (dd.IsInError)
					blnContinue = false;
				else
					blnContinue = !dd.IsLastItem;
				
				tw.Write(dd.DisplayAs);
				if (blnContinue)
					tw.Write(", ");
			}

			return tw;
		}
	}
}
