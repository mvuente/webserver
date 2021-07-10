package http_client;

import java.net.HttpURLConnection;
import java.net.URLConnection;
import java.net.URL;
import java.net.ProtocolException;
import java.io.DataOutputStream;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.BufferedReader;
import java.io.IOException;

public class HttpClient {

	private URLConnection connection;
	private final int TIME_TO_SLEEP = 1000;

	public void connect(String rawURL) throws IOException, ProtocolException
	{
		URL url = new URL(rawURL);
		connection = url.openConnection();
		//setRequestHeader("Connection", "Keep-Alive");
	}

	public void setRequestHeader(String key, String val) {
		connection.setRequestProperty(key, val);
	}

	public String getResponse() throws IOException{
		InputStream is = connection.getInputStream();
		BufferedReader rd = new BufferedReader(new InputStreamReader(is));
		StringBuilder response = new StringBuilder(); // or StringBuffer if Java version 5+
		String line;
		while ((line = rd.readLine()) != null) {
			response.append(line);
			response.append('\n');
		}
		rd.close();
		return response.toString();
	}

	public void connectNTimes(String rawURL, int n) throws IOException, ProtocolException, InterruptedException {
		for (int i = 0; i < n; ++i)
		{
			connect(rawURL);
			synchronized (System.out)
			{
				System.out.println(getResponse());
			}
			Thread.sleep(TIME_TO_SLEEP);
		}
	}

	public void closeConnection() {
		if (connection != null)
			((HttpURLConnection)connection).disconnect();
	}

	public static void main(String[] args) {
		HttpClient client = new HttpClient();
		try {
			client.connect("http://localhost:1028/");
			System.out.println(client.getResponse());
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			client.closeConnection();
		}
	}
}