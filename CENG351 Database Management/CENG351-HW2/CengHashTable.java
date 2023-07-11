import java.util.ArrayList;

public class CengHashTable {

	private int hashMode;
	private int globalDepth, size;
	private String hashStr, hashPrefix;
	private ArrayList<CengBucket> directory;
	//private ArrayList<CengHashRow> directory1;
	public CengHashTable()
	{
		// TODO: Create a hash table with only 1 row.
		hashMode  = CengPokeKeeper.getHashMod();
		directory = new ArrayList<CengBucket>();
		CengBucket headBucket = new CengBucket();  // new cengbucket object for
		directory.add(headBucket);
		size = 1;
		globalDepth = 0;  // global depth is 0 initially
		hashPrefix = "0";
	}

	public void deletePoke(Integer pokeKey)
	{
		boolean flag = false;
		int hashVal = hashFinder(pokeKey);
		CengBucket tmp = directory.get(hashVal);
		CengPoke mypoke = null;
		for(int j=0; j<tmp.bucket.size(); j++)
		{
			if(tmp.bucket.get(j).pokeKey() == pokeKey)
			{
				mypoke = tmp.bucket.get(j);
				flag = true;
			}
		}
		if(flag)
		{
			flag = false;
			tmp.removeData(mypoke);
		}



		int count = 0;
		ArrayList<CengBucket> holder = new ArrayList<CengBucket>();
		for(int i=0; i<size; i++)
		{
			CengBucket tmpB = directory.get(i);
			if (tmpB.bucket.size() == 0 && !holder.contains(tmpB))
			{
				holder.add(tmpB);
				count += 1;
			}
		}

		System.out.print("\n" + "\"delete\": {");
		System.out.print("\n\t" + "\"emptyBucketNum\": " + count);
		System.out.print("\n" + "}");
	}

	public void addPoke(CengPoke poke)
	{
		int theKey = hashFinder(poke.pokeKey());  // find the hashed value
		poke.setHashStr(hashStr);  // set hashed string but not truncated one
		if(directory.get(theKey).isFull())
		{
			directory.get(theKey).incr_depth();  // increment localdepth
			if(directory.get(theKey).getLocalDepth()<=globalDepth)
			{
				Split(theKey);
			}

			if(directory.get(theKey).getLocalDepth() > globalDepth)  // if bucket is full, double the directory
			{
				Doubler(theKey, poke.pokeKey());
			}
		}

		theKey = hashFinder(poke.pokeKey());
		directory.get(theKey).addData(poke);
	}
	
	public void searchPoke(Integer pokeKey)
	{
		Boolean flag = false;
		int prevcount = 0, count = 0;
		System.out.print("\n" + "\"search\": {");

		for(int i=0; i<size; i++)
		{
			int bSize = directory.get(i).bucket.size();


			if(directory.get(i).includes(pokeKey))
			{
				if(prevcount-count == -1) System.out.print(",");
				System.out.print("\n" + "\t" + "\"row\": {");
				System.out.print("\n" + "\t\t" + "\"hashPref\": " + prefGenerator(i) + ",");  // EDIT HERE

				System.out.print("\n" + "\t\t" + "\"bucket\": {");
				System.out.print("\n" + "\t\t\t" + "\"hashLength\": "  + directory.get(i).getLocalDepth() + ",");
				System.out.print("\n" + "\t\t\t" + "\"pokes\": [");
				for (int j=0; j<bSize; j++)
				{
					directory.get(i).bucket.get(j).printPoke();
					if(j != bSize-1) System.out.print(",");
				}
				System.out.print("\n" + "\t\t\t" + "]");
				System.out.print("\n" + "\t\t" + "}");
				System.out.print("\n" + "\t" + "}");
				flag = false;
				prevcount = count;
				count += 1;
			}
		}
		System.out.print("\n}");
	}
	
	public void print()
	{
		System.out.print("\n" + "\"table\": {");
		for(int i=0; i<size; i++)
		{
			System.out.print("\n" + "\t" + "\"row\": {");
			System.out.print("\n" + "\t\t" + "\"hashPref\": " + prefGenerator(i) + ",");  // EDIT HERE

			System.out.print("\n" + "\t\t" + "\"bucket\": {");
			System.out.print("\n" + "\t\t\t" + "\"hashLength\": "  + directory.get(i).getLocalDepth() + ",");
			System.out.print("\n" + "\t\t\t" + "\"pokes\": [");
			int bSize = directory.get(i).bucket.size();
			for (int j=0; j<bSize; j++)
			{
				directory.get(i).bucket.get(j).printPoke();
				if(j != bSize-1) System.out.print(",");
			}
			System.out.print("\n" + "\t\t\t" + "]");
			System.out.print("\n" + "\t\t" + "}");
			System.out.print("\n" + "\t" + "}");
			if(i != size-1) System.out.print(",");
		}
		System.out.print("\n" + "}");

		// TODO: Empty Implementation
	}

	///////////////////////////////
	public String prefGenerator(int num)
	{
		String s = Integer.toBinaryString(num);
		while(s.length()<globalDepth){
			s="0"+s;
		}
		return s;

	}
	public void incrGlobalDepth()  // increment when doubling
	{
		globalDepth += 1;
	}

	public int hashFinder(int key)  // pokemon key is parameter
	{
		int bitoffset = (Integer.toBinaryString(CengPokeKeeper.getHashMod()-1)).length();

		int temp = key % hashMode;
		hashStr = Integer.toBinaryString(temp);

		while(hashStr.length() < bitoffset){
			hashStr="0"+hashStr;
		}

		if(globalDepth != 0) hashPrefix = hashStr.substring(0, globalDepth);  // hashprefix is first -globaldepth- bits
		else hashPrefix = "0";
		int hashVal = Integer.parseInt(hashPrefix, 2);  // hashval is the index that we use in directory
		directory.get(hashVal).setHashPrefix(hashPrefix);
		return hashVal;
	}

	public void Split(int hashVal)
	{

		int ind1 = hashVal;
		int ind2;
		if(hashVal%2 == 0) ind2 = ind1+1;
		else ind2 = ind1-1;

		ArrayList<CengPoke> tmp = new ArrayList<CengPoke>();
		for(int i=0; i<CengPokeKeeper.getBucketSize(); i++)
		{
			tmp.add(directory.get(hashVal).bucket.get(i));  // copy bucket to the tmp
		}

		directory.get(ind1).removeAll();

		CengBucket newBucket = new CengBucket(directory.get(hashVal).getLocalDepth());

		directory.set(ind2, newBucket);

		for(int i=0; i<CengPokeKeeper.getBucketSize(); i++)
		{
			int theVal = hashFinder(tmp.get(i).pokeKey());
			directory.get(theVal).addData(tmp.get(i));
		}
	}

	public void Doubler(int hashVal, int key)
	{
		int prevsize = size;
		int bucSize = CengPokeKeeper.getBucketSize();
		size *= 2;
		incrGlobalDepth();

		//////// Split \\\\\\\\\
		ArrayList<CengBucket> tmp = new ArrayList<CengBucket>();
		for(int i=0; i<prevsize; i++)
		{
			tmp.add(directory.get(i));
			tmp.add(directory.get(i));
		}

		directory.clear();
		for(int i=0; i<size; i++)
		{
			directory.add(tmp.get(i));
		}

		hashVal = hashFinder(key);
		Split(hashVal);

		///////// Split is now finished. \\\\\\\\\\
	}

	public String getHashPref()
	{
		return hashPrefix;
	}
	///////////////////////////////
	// GUI-Based Methods
	// These methods are required by GUI to work properly.
	
	public int prefixBitCount()
	{
		// TODO: Return table's hash prefix length.
		return 0;
	}
	
	public int rowCount()
	{
		// TODO: Return the count of HashRows in table.

		return 0;
	}
	
	public CengHashRow rowAtIndex(int index)
	{
		// TODO: Return corresponding hashRow at index.
		return null;
	}
	
	// Own Methods
}
