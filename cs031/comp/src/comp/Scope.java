package comp;

import java.util.*;

/**
 * A Scope is a dictionary of identifiers and their types. Given an identifier
 * and its type, it can tell you whether the identifier has
 * been declared with that type. You insert a variable into the scope when it is
 * declared, and you look up the variable when it is used.
 * <p>
 * Usage examples: <code>
 * int a; // call insert ("a", Scope.VarType.SINGLE);
 * int array[4]; // call insert ("array", Scope.VarType.ARRAY);
 * a = 5; // make sure lookup("a") is SINGLE
 * b = 9; // lookup("b") will be UNTYPED because b has not been declared
 * </code>
 * 
 * @version Sat Sep 02 2006
 * @modifier Daniel Winograd-Cort
 */
public class Scope {
	
	//UNTYPED will return if the identifier was never created
	public enum VarType{SINGLE,ARRAY,UNTYPED};
	
	private Map<String, VarType> _varList;
	
	/**
	 * Make an empty scope.
	 */
	public Scope() {
		_varList = new HashMap<String, VarType>();
	}
	
	/**
	 * Add an identifier to the scope. Fails if the identifier is already
	 * in scope, regardless of type.
	 * 
	 * @param name
	 *           Identifier being declared
	 * @param type
	 *           The type of the variable
	 * @return True if insert was successful.
	 */
	public boolean insert(String name, VarType type) {
		// make sure the variable name is not used
		if (!_varList.containsKey(name)) {
			// insert variable
			_varList.put(name,type);
			return true;
		} else {
			return false;
		}
	}
	
	/**
	 * See if this identifier is in scope and what type it has.
	 * 
	 * @param name Identifier being used in program
	 * @return the type of the variable or untyped if it was not defined
	 */
	public VarType lookup(String name) {
		VarType result = _varList.get(name);
		return (result == null ? VarType.UNTYPED : result);
	}
}
