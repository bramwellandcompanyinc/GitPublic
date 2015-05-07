using System;
using System.Collections.Generic;
using FeedMeThese.Entities;
using Microsoft.Practices.ObjectBuilder2;
using FeedMeThese.Interfaces;

namespace FeedMeThese.Implementations
{
	public class ParsedOutput : IParsedOutput
	{
		private bool _inError = false;
		private List<IDishDisplay> _dishes = null;

		public ParsedOutput()
		{
			Clear();
		}

		
		//Provided to make sure that we can clear the class out between successive
		//uses.

		public void Clear()
		{
			_inError = false;
			_dishes = new List<IDishDisplay>();
		}

		public void Add(DishDefinition dish)
		{
			//This class contains no understanding of whether the insertion is correct as
			//per business rules; if it is being added here it is assumed that the
			//relevant business rule has already been passed.

			IDishDisplay dd = FindDish(dish);
			if (dd != null)
				dd.Count++;
			else
				_dishes.Add(new DishDisplay(1, false, dish));
		}


		//Set this class into an error state so that the rendering of the information
		//is aware that error has to be flagged to the user.

		public void SetErrorState()
		{
			//We're going to add an item to indicate the error state.
			//Because the DishDisplay class knows how to render itself, it will
			//make it easier to render errors because it is treated, for all intents
			//and purposes, as a regular dish item during rendering.

			if (!_inError)
			{
				_inError = true;
				_dishes.Add(new DishDisplay(0, true, null));	//Create a blank "dummy" error dish display item
			}
		}

		public bool IsInError()
		{
			return _inError;
		}


		//Retrieve the current number of items for a given dish type

		public uint CountOf(DishDefinition dish)
		{
			IDishDisplay dd = FindDish(dish);
			if (dd != null)
				return dd.Count;
			else
				return 0;
		}


		//Render the structure into a human-readable output string

		public override string ToString()
		{
			string output = "";

			//Invoke the ToString method on each of the items
			//Sort them first using the comparer for DishDisplay

			if (_dishes.Count > 0)
			{
				_dishes.Sort(_dishes[0].Comparer);
				output = _dishes.JoinStrings(", ");
			}

			return output;
		}


		//Find an instance of a specific dish in our list

		private IDishDisplay FindDish(DishDefinition dish)
		{
			return _dishes.Find(d => d.DefinitionRef.Id == dish.Id);
		}
	}
}
