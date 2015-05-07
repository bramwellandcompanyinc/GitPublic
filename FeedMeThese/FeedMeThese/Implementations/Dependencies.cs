using FeedMeThese.Entities;
using FeedMeThese.Implementations;
using FeedMeThese.Interfaces;
using Microsoft.Practices.Unity;

namespace FeedMeThese.DependencyInjection
{
	public static class Dependencies
	{
		private static IUnityContainer _container = new UnityContainer();

		public static void EstablishContainer()
		{
			_container.RegisterType<IInputParser, InputParser>();
			_container.RegisterType<IParsedOutput, ParsedOutput>();
			_container.RegisterType<IRepository<DishDefinition>, StaticRepository>();
		}

		public static T Resolve<T>()
		{
			return _container.Resolve<T>();
		}
	}
}
