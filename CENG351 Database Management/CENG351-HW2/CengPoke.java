import java.util.Locale;

public class CengPoke {
	
	private Integer pokeKey;
	private String hashPref;
	private String pokeName;
	private String pokePower;
	private String pokeType;
	private String hashStr;
	
	public CengPoke(Integer pokeKey, String pokeName, String pokePower, String pokeType)
	{
		this.pokeKey = pokeKey;
		this.pokeName = pokeName;
		this.pokePower = pokePower;
		this.pokeType = pokeType;
	}
	
	// Getters
	
	public Integer pokeKey()
	{
		return pokeKey;
	}
	public String pokeName()
	{
		return pokeName;
	}
	public String pokePower()
	{
		return pokePower;
	}
	public String pokeType()
	{
		return pokeType;
	}

	public void printPoke()
	{
		System.out.print("\n" + "\t\t\t\t" + "\"poke\": {");
		System.out.print("\n" + "\t\t\t\t\t" + "\"hash\": " + hashStr + ",");
		System.out.print("\n" + "\t\t\t\t\t" + "\"pokeKey\": " + pokeKey + ",");
		System.out.print("\n" + "\t\t\t\t\t" + "\"pokeName\": " + pokeName + ",");
		System.out.print("\n" + "\t\t\t\t\t" + "\"pokePower\": " + pokePower + ",");
		System.out.print("\n" + "\t\t\t\t\t" + "\"pokeType\": " + pokeType);
		System.out.print("\n" + "\t\t\t\t" + "}");
	}

	public void setHashStr(String H)
	{
		hashStr = H;
	}
	// GUI method - do not modify
	public String fullName()
	{
		return "" + pokeKey() + "\t" + pokeName() + "\t" + pokePower() + "\t" + pokeType;
	}
		
	// Own Methods

	public void setHashPref(String hPref)
	{
		hashPref = hPref;
	}

	public String getHashPref()
	{
		return hashPref;
	}
}
