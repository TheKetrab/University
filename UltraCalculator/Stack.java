
import java.io.*;
import java.util.*;

public class Stack<T> {

	class Elem<T> { 
		private Elem<T> prev;
		private T val;

		public Elem(T val) {
			prev = null;
			this.val = val;
		}
		
		public Elem(T val, Elem<T> prev) {
			this.prev = prev;
			this.val = val;
		}
		
	}

	private Elem<T> top;

	public Stack() {
		top = null;
	}
	
	public Stack(T val) {
		top = new Elem<T>(val);
	}
	
	public void push(T val) {
		top = new Elem<T>(val,top);
	}
	
	public T pop() {
		T res = top.val;
		top = top.prev;
		return res;
	}
	
	public T top() {
		return top.val;
	}
	
	public Boolean empty() {
		if (top == null) return true;
		return false;
	}
	
	public void write() {
		Elem<T> temp = top;
		
		System.out.print("TOP -> [ ");
		while (temp != null) {
			System.out.print(temp.val + " ");
			temp = temp.prev;
		}
		
		System.out.print("]\n");

	}
	
	
	
}
