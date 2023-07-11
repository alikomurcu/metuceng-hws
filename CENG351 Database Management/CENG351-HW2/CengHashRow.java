public class CengHashRow {

	// GUI-Based Methods
	// These methods are required by GUI to work properly.
	private int hashMode = CengPokeKeeper.getHashMod();
	private String hashPref;

	public CengHashRow(String hashP)
	{
		hashPref = hashP;
	}
	public String hashPrefix()
	{
		// TODO: Return row's hash prefix (such as 0, 01, 010, ...)

		return hashPref;
	}
	
	public CengBucket getBucket()
	{
		// TODO: Return the bucket that the row points at.

		return null;		
	}
	
	public boolean isVisited()
	{
		// TODO: Return whether the row is used while searching.
		return false;		
	}
	
	// Own Methods
}
