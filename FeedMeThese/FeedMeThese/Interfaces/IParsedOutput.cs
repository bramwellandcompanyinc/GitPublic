using FeedMeThese.Entities;
namespace FeedMeThese.Interfaces
{
	public interface IParsedOutput
	{
		void Clear();
		void Add(DishDefinition dish);
		void SetErrorState(); //we don't expose this as a property because once it is set we don't want it being reset
		bool IsInError();
		uint CountOf(DishDefinition dish);
		string ToString();
	}
}