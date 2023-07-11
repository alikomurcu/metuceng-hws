import java.util.ArrayList;

public class CengBucket {

	// GUI-Based Methods
	// These methods are required by GUI to work properly.
	private int capacity;
	private int depth;
	private Boolean fullF;
	private String hashPrefix;
	ArrayList<CengPoke> bucket;

	public CengBucket()
	{
		capacity = CengPokeKeeper.getBucketSize();
		bucket = new ArrayList<CengPoke>();
		fullF = false;
		depth = 0;
	}

	public CengBucket(int depth)
	{
		capacity = CengPokeKeeper.getBucketSize();
		bucket = new ArrayList<CengPoke>();
		fullF = false;
		this.depth = depth;
	}

	public CengBucket(CengBucket cp)  // copy constructor
	{
		cp.capacity = this.capacity;
		cp.depth = this.depth;
		cp.fullF = this.fullF;
		for (int i=0; i<capacity; i++)
		{
			cp.bucket.set(i, this.bucket.get(i));  // deep copy of mutable object
		}
	}

	public Boolean includes(int key)
	{
		for(int i=0; i< bucket.size(); i++)
		{
			if(bucket.get(i).pokeKey() == key)
			{
				return true;
			}
		}
		return false;
	}

	public void addData(CengPoke data)  // adds the cengpoke object to bucket if size is enough
	{
		if(bucket.size() < capacity)
		{
			bucket.add(data);
		}
		if(bucket.size() == capacity)
		{
			fullF = true;
		}
	}

	public void removeAll()
	{
		while(bucket.size() != 0)
		{
			bucket.remove(0);
			fullF = false;
		}
	}

	public void removeData(CengPoke data)
	{
		bucket.remove(data);
		fullF = false;
	}

	public void setHashPrefix(String H)
	{
		hashPrefix = H;
	}

	public String getMyPrefixNow()
	{
		return hashPrefix;
	}

	public Boolean isFull()
	{
		return fullF;
	}

	public void incr_depth()  // when split occurs
	{
		depth += 1;
	}

	public int getLocalDepth()
	{
		return depth;
	}
	public int pokeCount()
	{
		// TODO: Return the pokemon count in the bucket.
		return bucket.size();
	}
	
	public CengPoke pokeAtIndex(int index)
	{
		// TODO: Return the corresponding pokemon at the index.
		return bucket.get(index);
	}
	
	public int getHashPrefix()
	{
		// TODO: Return hash prefix length.
		return depth;
	}
	
	public Boolean isVisited()
	{
		// TODO: Return whether the bucket is found while searching.
		return false;
		// classlar yazildiktan sonra buna tekrar bakilacak
	}
	
	// Own Methods
}
