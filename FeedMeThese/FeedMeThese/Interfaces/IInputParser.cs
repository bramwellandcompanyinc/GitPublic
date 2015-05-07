using FeedMeThese.Interfaces;
using Microsoft.Practices.Unity;

namespace FeedMeThese.Interfaces
{
	public interface IInputParser
	{
		IParsedOutput ParseInput(string input);
	}
}
