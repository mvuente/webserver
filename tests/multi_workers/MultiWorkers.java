package multi_workers;
import http_client.HttpClient;
import java.time.format.DateTimeFormatter;  
import java.time.LocalDateTime;    
import java.util.Map;
import java.util.Set;
import java.util.HashMap;
import java.util.HashSet;

public class MultiWorkers {
	Thread[] workers;
	Worker[] wObjs;
	public void createNWorkers(int n, int max_req) {
		wObjs 	= new Worker[n];
		workers = new Thread[n];
		for (int i = 0; i < workers.length; ++i)
		{
			wObjs[i]	= new Worker(i, max_req);
			workers[i]	= new Thread(wObjs[i]);
		}
	}

	private static boolean isDone(Set<Integer> ndone, int nWorkers) {
		System.out.println(ndone.size());
		return ndone.size() == nWorkers;
	}

	public void start() {
		Set<Integer> ndone = new HashSet<Integer>();

		for (int i = 0; i < workers.length; ++i)
			workers[i].start();
	
		main_loop:
		while (!isDone(ndone, workers.length))
		{
			for (int i = 0; i < workers.length; ++i)
			{
				if (!workers[i].isAlive())
				{
					if (wObjs[i].isOver())
						ndone.add(i);
					else
						break main_loop;
				}
			}
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
				
			}
		}
		System.out.println("HERE I AM");
		if (isDone(ndone, workers.length))
		{
			System.out.println("Finished successfully");
			System.exit(1);
		}
		else
		{
			System.out.println("ERROR!");
			System.exit(1);
		}
	}
}

class Worker implements Runnable {
	private int				id = 0;
	private int				max_request;
	private int				ndone = 0;

	public Worker()
	{
		max_request = 100;
	}

	public Worker(int aID, int max_req) {
		max_request = max_req;
		id = aID;
	}

	void setMaxRequest(int max_req) {
		max_request = max_req;
	}

	boolean isOver() {
		return max_request == ndone;
	}

	public void run() {
		DateTimeFormatter dtf = DateTimeFormatter.ofPattern("yyyy/MM/dd HH:mm:ss");  
		LocalDateTime now;
	
		for (int i = 0; i < max_request; ++i)
		{
			HttpClient client = new HttpClient();
			try {
				client.connect("http://localhost:1028");
				synchronized (System.out) {
					client.getResponse();
					now = LocalDateTime.now();
					System.out.printf("[%s] %d): %d\n", dtf.format(now), id, ++ndone);
				}
			} catch (Exception e) {
				e.printStackTrace();
			} finally {
				client.closeConnection();
			}
		}
	}
}