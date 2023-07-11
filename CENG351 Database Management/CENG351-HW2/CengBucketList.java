import java.util.ArrayList;

public class CengBucketList {
	private int hashMode;
	private int globalDepth, size;
	private String hashStr, hashPrefix;
	private ArrayList<CengBucket> directory;

	public CengBucketList()
	{
		// TODO: Create a hash table with only 1 row.
		hashMode  = CengPokeKeeper.getHashMod();
		directory = new ArrayList<CengBucket>();
		CengBucket headBucket = new CengBucket();  // new cengbucket object for
		directory.add(headBucket);
		size = 1;
		globalDepth = 0;  // global depth is 0 initially
	}
	public void deletePoke(Integer pokeKey)
	{
		// TODO: Empty Implementation
	}

	public void addPoke(CengPoke poke)
	{
		int theKey = hashFinder(poke.pokeKey());  // find the hashed value
		if(directory.get(theKey).isFull())  // if bucket is full, double the directory
		{
			Doubler(theKey);
		}

		directory.get(theKey).addData(poke);
		// TODO: Empty Implementation
	}

	public int hashFinder(int key)  // pokemon key is parameter
	{
		if(globalDepth == 0)
			return 0;
		int temp = key % hashMode;
		hashStr = Integer.toBinaryString(temp);
		hashPrefix = hashStr.substring(0, globalDepth);  // hashprefix is first -globaldepth- bits
		int hashVal = Integer.parseInt(hashPrefix, 2);  // hashval is the index that we use in directory
		return hashVal;
	}

	public void Doubler(int hashVal)
	{
		int prevsize = size;
		int bucSize = CengPokeKeeper.getBucketSize();
		size *= 2;
		incrGlobalDepth();

		//////// Split \\\\\\\\\
		for(int i=prevsize; i<size; i++)
		{
			if(i != hashVal)  // i not equal to the index that will be inserted
			{
				directory.set(i, directory.get(i-prevsize));  // shallow copy of the buckets
			}
		}

		CengBucket cp = new CengBucket(directory.get(hashVal));  // deep copy the current bucket
		directory.set(hashVal+prevsize, cp);  // set the deep copied bucket

		int theKey;
		ArrayList<CengPoke> tmp = new ArrayList<CengPoke>();
		for(int i=0; i<bucSize; i++)  // copy array to a temporary array and then split
		{
			tmp.add(directory.get(i).bucket.get(i));  // now all entries are copied from directory->bucket to tmp
		}
		directory.get(hashVal).removeAll();  // remove all entries

		CengBucket curBucket = directory.get(hashVal);
		for (int i=0; i<bucSize; i++)
		{
			theKey = hashFinder(curBucket.pokeAtIndex(i).pokeKey());
			directory.get(theKey).addData(tmp.get(i));  // now all entires are added to hashtable. each one is forwarded to corresponding directory->bucket
		}
		///////// Split is now finished. \\\\\\\\\\
	}

	public void incrGlobalDepth()  // increment when doubling
	{
		globalDepth += 1;
	}

	public void searchPoke(Integer pokeKey)
	{
		// TODO: Empty Implementation
	}
	
	public void print()
	{
		// TODO: Empty Implementation
	}
	
	// GUI-Based Methods
	// These methods are required by GUI to work properly.
	
	public int bucketCount()
	{
		// TODO: Return all bucket count.
		return 0;		
	}
	
	public CengBucket bucketAtIndex(int index)
	{
		// TODO: Return corresponding bucket at index.
		return null;
	}
	
	// Own Methods
}
