import multi_workers.MultiWorkers;

class Tester {
	public static void main(String[] args) {
		final int nWorkers		= args.length > 0 ? Integer.parseInt(args[0]) : 5;
		final int max_request	= args.length > 1 ? Integer.parseInt(args[1]) : 20;
		MultiWorkers ws = new MultiWorkers();

		ws.createNWorkers(nWorkers, max_request);
		ws.start();
	}
}