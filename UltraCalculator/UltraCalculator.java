import java.awt.EventQueue;

import javax.swing.JFrame;
import javax.swing.BoxLayout;
import javax.swing.DefaultListModel;

import java.awt.GridLayout;
import javax.swing.JTextField;
import java.awt.BorderLayout;
import javax.swing.JTextPane;
import javax.swing.SwingConstants;

import java.awt.GridBagLayout;
import javax.swing.JPanel;
import javax.swing.JButton;
import javax.swing.JLabel;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import javax.swing.JSlider;
import javax.swing.JScrollPane;
import javax.swing.JScrollBar;
import javax.swing.JList;
import java.awt.Scrollbar;
import java.awt.ScrollPane;


import java.awt.Color;
import java.awt.Font;
import java.awt.*;
import java.awt.font.*;
import java.io.*;
import java.util.List;

import javax.swing.*;


public class UltraCalculator {

	private JFrame frame;
	private JTextField textField;
	private Memory mem;
	
	
	/**
	 * Launch the application.
	 */
	public static void main(String[] args) {
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				try {
					UltraCalculator window = new UltraCalculator();
					window.frame.setVisible(true);
					

					
					//window.setSize(400,400);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}
	
	
// ***** ***** ***** ***** *****
	public DefaultListModel<String> init_varlist(List<Var> lst) {
			
		DefaultListModel<String> res = new DefaultListModel<String>();
		
		for(Var object: lst) {
			res.addElement(object.Letter + " := " + object.Meaning);
		}

		return res;
	}
	
	public DefaultListModel<String> init_funclist(List<Func> lst) {
		
		DefaultListModel<String> res = new DefaultListModel<String>();
		
		for(Func object: lst) {
			res.addElement(object.Name + "(" + object.Arity + " args) := " + object.Body);
		}

		return res;
	}
	
	
	/**
	 * Create the application.
	 */
	public UltraCalculator() {
		
		mem = new Memory();
		initialize();
		
	}

	/**
	 * Initialize the contents of the frame.
	 */
	private void initialize() {
		Font font_of_text 		= new Font("SansSerif", Font.BOLD, 40);
		Font font_of_smalltext 		= new Font("SansSerif", Font.BOLD, 12);
		Font font_of_label 		= new Font("SansSerif", Font.BOLD, 20);
		Font font_of_buttons 	= new Font("ARIAL", Font.PLAIN, 15);
		Font font_of_items 		= new Font("SansSerif", Font.BOLD, 20);
		

		frame = new JFrame();		
		frame.setPreferredSize(new Dimension(605, 465)); //440
	    frame.pack();
		frame.setLocationRelativeTo(null);
		frame.setResizable(false);
		
		// PASEK MENU
		JMenuBar menuBar = new JMenuBar();//menu
	    frame.setJMenuBar(menuBar);

	    JMenuItem authorChoice = new JMenuItem("Author");
	    authorChoice.addActionListener(new ActionListener() {
	        public void actionPerformed(ActionEvent evt) {
	            
	        	JDialog AuthorDialog = new JDialog();
	        	AuthorDialog.setLayout(null);
	        	AuthorDialog.setPreferredSize(new Dimension(300, 200));
	        	AuthorDialog.setLocationRelativeTo(null);
	        	AuthorDialog.setResizable(false);
				

				// INFO
				JLabel AuthorDialog_label_Info = new JLabel("<html>O autorze:<br>"
														+ "Projekt wykonał Bartłomiej Grochowski<br> "
														+ "w ramach przedmiotu Programowanie Obiektowe "
														+ "prowadzonego w Instytucie Informatyki "
														+ "Uniwersytetu Wrocławskiego."
														+ "");
				AuthorDialog_label_Info.setBounds(5, 5, 290, 150);
				AuthorDialog_label_Info.setFont(font_of_smalltext);
				AuthorDialog_label_Info.setHorizontalAlignment(JTextField.LEFT);
				AuthorDialog.add(AuthorDialog_label_Info);
			
				
				
				AuthorDialog.setLocationRelativeTo(null);
				AuthorDialog.setTitle("Autnor.");
				AuthorDialog.pack();
				AuthorDialog.setVisible(true);
		        
				
	        	
	        	
	        }
	    });
	    menuBar.add(authorChoice);
	    
	    JMenuItem licenseChoice = new JMenuItem("License");
	    licenseChoice.addActionListener(new ActionListener() {
	        public void actionPerformed(ActionEvent evt) {
	            
	        	JDialog LicenseDialog = new JDialog();
	        	LicenseDialog.setLayout(null);
	        	LicenseDialog.setPreferredSize(new Dimension(300, 200));
	        	LicenseDialog.setLocationRelativeTo(null);
	        	LicenseDialog.setResizable(false);
				

				// INFO
				JLabel LicenseDialog_label_Info = new JLabel("<html>Licencja:<br>"
														+ "Do wolnego użytkowania oraz rozszerzania "
														+ "aplikacji na własny użytek. Zabrania się  "
														+ "udostępniania aplikacji publicznie."
														+ "");
				LicenseDialog_label_Info.setBounds(5, 5, 290, 150);
				LicenseDialog_label_Info.setFont(font_of_smalltext);
				LicenseDialog_label_Info.setHorizontalAlignment(JTextField.LEFT);
				LicenseDialog.add(LicenseDialog_label_Info);
			
				
				
				LicenseDialog.setLocationRelativeTo(null);
				LicenseDialog.setTitle("Licencja.");
				LicenseDialog.pack();
				LicenseDialog.setVisible(true);
		        
				
	        	
	        	
	        }
	    });
	    menuBar.add(licenseChoice);
	    
	    // ----- ----- ----- ----- -----

	    JMenuItem version = new JMenuItem("Version 1.0");
	    menuBar.add(version);
	    
		
		
		JPanel panel = new JPanel();
		frame.getContentPane().add(panel);
		panel.setLayout(null);
		
		textField = new JTextField();
		textField.setBounds(5, 5, 590, 120);
	    textField.setFont(font_of_text);
		textField.setHorizontalAlignment(JTextField.RIGHT);
		panel.add(textField);
		
		
		Action action_enter = new AbstractAction()
		{
		    @Override
		    public void actionPerformed(ActionEvent e)
		    {
				String got = textField.getText();
				double res_of_alg = Rpn.expr_to_result(got,mem);
				//
				textField.setText(Double.toString(res_of_alg));
		    }
		};
		textField.addActionListener( action_enter );
		
		
		
		
		
		

		JButton btnHelp = new JButton("Help");
		btnHelp.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				
				JDialog HelpDialog = new JDialog();
	        	HelpDialog.setLayout(null);
	        	HelpDialog.setPreferredSize(new Dimension(300, 200));
	        	HelpDialog.setLocationRelativeTo(null);
	        	HelpDialog.setResizable(false);
				

				// INFO
				JLabel HelpDialog_label_Info = new JLabel("<html>POMOC:<br>"
														+ "Do kalkulatora należy wpisywać "
														+ "poprawne wyrażenia arytmetyczne. "
														+ "Można używać skrótów (np. 2ab). "
														+ "Należy trzymać się konwencji: "
														+ "zmienne to małe litery, "
														+ "funkcje to wielkie litery. "
														+ "Sposób na dodawanie funkcji/zmiennych "
														+ "jest opisany bezpośrednio przy tym procesie. "
														+ "UWAGA: program nie razie sobie z niepoprawnymi"
														+ "wyrażeniami arytmetycznymi.");
				HelpDialog_label_Info.setBounds(5, 5, 290, 150);
				HelpDialog_label_Info.setFont(font_of_smalltext);
				HelpDialog_label_Info.setHorizontalAlignment(JTextField.LEFT);
				HelpDialog.add(HelpDialog_label_Info);
			
				
				
				HelpDialog.setLocationRelativeTo(null);
				HelpDialog.setTitle("Pomoc.");
				HelpDialog.pack();
				HelpDialog.setVisible(true);
			}
		});
		
		btnHelp.setBounds(455, 335, 140, 25);
		btnHelp.setFont(font_of_buttons);
		panel.add(btnHelp);
		
		JButton btnExit = new JButton("Exit");
		btnExit.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				frame.dispose();
				
			}
		});
		
		btnExit.setBounds(455, 375, 140, 25);
		btnExit.setFont(font_of_buttons);
		panel.add(btnExit);
		
		/* ----- -----  -----  ----- ----- */
		/* ***** ***** VarList ***** ***** */
		/* ----- -----  -----  ----- ----- */

		DefaultListModel VarListModel = init_varlist(mem.vars);
		JList VarList_JList = new JList(VarListModel);
		JScrollPane VarList = new JScrollPane(VarList_JList);
		
		VarList_JList.setLayoutOrientation(JList.VERTICAL);
		VarList_JList.setVisibleRowCount(20);
		VarList.setBounds(5, 175, 140, 150);
		panel.add(VarList);
	
		JButton btnAddVar = new JButton("Add Var");
		btnAddVar.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				
				// ----- ----- DIALOG ----- -----
				JDialog VarDialog = new JDialog();
				VarDialog.setLayout(null);
				VarDialog.setPreferredSize(new Dimension(300, 200));
				VarDialog.setLocationRelativeTo(null);
				VarDialog.setResizable(false);
				

				// INFO
				JLabel VarDialog_label_Info = new JLabel("<html>To add a variable, you have to fill every textfield<br>"
														+ "correctly. You have to follow this template:<br>"
														+ "<center>x , 2+5*3 etc.");
				VarDialog_label_Info.setBounds(5, 5, 290, 50);
				VarDialog_label_Info.setFont(font_of_smalltext);
				VarDialog_label_Info.setHorizontalAlignment(JTextField.LEFT);
				VarDialog.add(VarDialog_label_Info);
				
				
				// NAME
				JLabel VarDialog_label_Name = new JLabel("Letter");
				VarDialog_label_Name.setBounds(5, 65, 110, 30);
				VarDialog_label_Name.setFont(font_of_smalltext);
				VarDialog_label_Name.setHorizontalAlignment(JTextField.CENTER);
				VarDialog.add(VarDialog_label_Name);
				
				JTextField VarDialog_text_Name = new JTextField();
				VarDialog_text_Name.setBounds(5, 90, 110, 30);
				VarDialog_text_Name.setFont(font_of_smalltext);
				VarDialog_text_Name.setHorizontalAlignment(JTextField.CENTER);
				VarDialog.add(VarDialog_text_Name);
				
				// BODY
				JLabel VarDialog_label_Body = new JLabel("Meaning");
				VarDialog_label_Body.setBounds(125, 65, 110, 30);
				VarDialog_label_Body.setFont(font_of_smalltext);
				VarDialog_label_Body.setHorizontalAlignment(JTextField.CENTER);
				VarDialog.add(VarDialog_label_Body);
				
				JTextField VarDialog_text_Body = new JTextField();
				VarDialog_text_Body.setBounds(125, 90, 110, 30);
				VarDialog_text_Body.setFont(font_of_smalltext);
				VarDialog_text_Body.setHorizontalAlignment(JTextField.CENTER);
				VarDialog.add(VarDialog_text_Body);
				
				
				
				
				JButton btnAddVar_Inside = new JButton("Add Var");
				btnAddVar_Inside.addActionListener(new ActionListener() {
					public void actionPerformed(ActionEvent e) {
						
						String l = VarDialog_text_Name.getText();
						String m = VarDialog_text_Body.getText();

						Var v = new Var(l,m);
						mem.add_var(v);
						
						VarListModel.addElement(v.Letter + " := " + v.Meaning);
						VarDialog.dispose();

					}
				});
				btnAddVar_Inside.setVisible(true);
				btnAddVar_Inside.setBounds(80, 130, 140, 25);
				btnAddVar_Inside.setFont(font_of_buttons);
				VarDialog.add(btnAddVar_Inside);
				
				
				
				
				VarDialog.setLocationRelativeTo(null);
				VarDialog.setTitle("Adding variable.");
				VarDialog.pack();
				VarDialog.setVisible(true);
		        
				
			}
		});
		
		btnAddVar.setBounds(5, 335, 140, 25);
		btnAddVar.setFont(font_of_buttons);
		panel.add(btnAddVar);
		
		JButton btnRemoveVar = new JButton("Remove Var");
		btnRemoveVar.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				
				int selectedIndex = VarList_JList.getSelectedIndex();
				if (selectedIndex != -1) {
				    VarListModel.remove(selectedIndex);
				    mem.vars.remove(selectedIndex);
				}
				
			}
		});
		
		btnRemoveVar.setBounds(5, 375, 140, 25);
		btnRemoveVar.setFont(font_of_buttons);
		panel.add(btnRemoveVar);
		// ----- ----- ----- ----- -----
		
		
		/* ----- -----   -----   ----- ----- */
		/* ***** ***** FuncList  ***** ***** */
		/* ----- -----   -----   ----- ----- */

		DefaultListModel FuncListModel = init_funclist(mem.funcs);
		JList FuncList_JList = new JList(FuncListModel);
		JScrollPane FuncList = new JScrollPane(FuncList_JList);
		
		FuncList_JList.setLayoutOrientation(JList.VERTICAL);
		FuncList_JList.setVisibleRowCount(20);
		FuncList.setBounds(155, 175, 140, 150);
		panel.add(FuncList);
	
		JButton btnAddFunc = new JButton("Add Func");
		btnAddFunc.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				
				// ----- ----- DIALOG ----- -----
				JDialog FuncDialog = new JDialog();
				FuncDialog.setLayout(null);
				FuncDialog.setPreferredSize(new Dimension(300, 200));
				FuncDialog.setLocationRelativeTo(null);
				FuncDialog.setResizable(false);
				

				// INFO
				JLabel FuncDialog_label_Info = new JLabel("<html>To add a function, you have to fill every textfield<br>"
														+ "correctly. You have to follow this template:<br>"
														+ "<center>FUNCNAME , (A+B)^SIN(C) etc. , n");
				FuncDialog_label_Info.setBounds(5, 5, 290, 50);
				FuncDialog_label_Info.setFont(font_of_smalltext);
				FuncDialog_label_Info.setHorizontalAlignment(JTextField.LEFT);
				FuncDialog.add(FuncDialog_label_Info);
				
				
				// NAME
				JLabel FuncDialog_label_Name = new JLabel("Name");
				FuncDialog_label_Name.setBounds(5, 65, 110, 30);
				FuncDialog_label_Name.setFont(font_of_smalltext);
				FuncDialog_label_Name.setHorizontalAlignment(JTextField.CENTER);
				FuncDialog.add(FuncDialog_label_Name);
				
				JTextField FuncDialog_text_Name = new JTextField();
				FuncDialog_text_Name.setBounds(5, 90, 110, 30);
				FuncDialog_text_Name.setFont(font_of_smalltext);
				FuncDialog_text_Name.setHorizontalAlignment(JTextField.CENTER);
				FuncDialog.add(FuncDialog_text_Name);
				
				// BODY
				JLabel FuncDialog_label_Body = new JLabel("Body");
				FuncDialog_label_Body.setBounds(125, 65, 110, 30);
				FuncDialog_label_Body.setFont(font_of_smalltext);
				FuncDialog_label_Body.setHorizontalAlignment(JTextField.CENTER);
				FuncDialog.add(FuncDialog_label_Body);
				
				JTextField FuncDialog_text_Body = new JTextField();
				FuncDialog_text_Body.setBounds(125, 90, 110, 30);
				FuncDialog_text_Body.setFont(font_of_smalltext);
				FuncDialog_text_Body.setHorizontalAlignment(JTextField.CENTER);
				FuncDialog.add(FuncDialog_text_Body);
				
				// ARITY
				JLabel FuncDialog_label_Arity = new JLabel("Arity");
				FuncDialog_label_Arity.setBounds(245, 65, 50, 30);
				FuncDialog_label_Arity.setFont(font_of_smalltext);
				FuncDialog_label_Arity.setHorizontalAlignment(JTextField.CENTER);
				FuncDialog.add(FuncDialog_label_Arity);
				
				JTextField FuncDialog_text_Arity = new JTextField();
				FuncDialog_text_Arity.setBounds(245, 90, 50, 30);
				FuncDialog_text_Arity.setFont(font_of_smalltext);
				FuncDialog_text_Arity.setHorizontalAlignment(JTextField.CENTER);
				FuncDialog.add(FuncDialog_text_Arity);
				
				
				JButton btnAddFunc_Inside = new JButton("Add Func");
				btnAddFunc_Inside.addActionListener(new ActionListener() {
					public void actionPerformed(ActionEvent e) {
						
						String n = FuncDialog_text_Name.getText();
						String b = FuncDialog_text_Body.getText();
						int a = Integer.parseInt(FuncDialog_text_Arity.getText());
						
						Func f = new Func(n,b,a);
						mem.add_func(f);
						
						FuncListModel.addElement(f.Name + "(" + f.Arity + " args) := " + f.Body);
						FuncDialog.dispose();

					}
				});
				btnAddFunc_Inside.setVisible(true);
				btnAddFunc_Inside.setBounds(80, 130, 140, 25);
				btnAddFunc_Inside.setFont(font_of_buttons);
				FuncDialog.add(btnAddFunc_Inside);
				
				
				
				
				FuncDialog.setLocationRelativeTo(null);
				FuncDialog.setTitle("Adding function.");
				FuncDialog.pack();
				FuncDialog.setVisible(true);
		        
				
				
			}
		});
		
		btnAddFunc.setBounds(155, 335, 140, 25);
		btnAddFunc.setFont(font_of_buttons);
		panel.add(btnAddFunc);
		
		JButton btnRemoveFunc = new JButton("Remove Func");
		btnRemoveFunc.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {

				int selectedIndex = FuncList_JList.getSelectedIndex();
				if (selectedIndex != -1) {
				    FuncListModel.remove(selectedIndex);
				    mem.funcs.remove(selectedIndex);
				}
				
				
			}
		});
		
		btnRemoveFunc.setBounds(155, 375, 140, 25);
		btnRemoveFunc.setFont(font_of_buttons);
		panel.add(btnRemoveFunc);
		
		
		
		// ----- ----- ----- ----- -----
				
		
		
		

		
		JLabel lblMemory = new JLabel("MEMORY");
		lblMemory.setBounds(10, 135, 290, 30);
		lblMemory.setHorizontalAlignment(JLabel.CENTER);
		lblMemory.setVerticalAlignment(JLabel.CENTER);
		lblMemory.setFont(font_of_label);
		panel.add(lblMemory);
		

		/* ----- -----   -----   ----- ----- */
		/* ***** ***** CalcButtons  ***** ***** */
		/* ----- -----   -----   ----- ----- */

		JButton button_bra1 = new JButton("(");
		button_bra1.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				String got = textField.getText();
				textField.setText(got+"(");
			}
		});
		button_bra1.setBounds(305, 135, 50, 40);
		panel.add(button_bra1);

		JButton button_bra2 = new JButton(")");
		button_bra2.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				String got = textField.getText();
				textField.setText(got+")");
			}
		});
		button_bra2.setBounds(365, 135, 50, 40);
		panel.add(button_bra2);
		
		JButton button_mod = new JButton("<html>&radic;");
		button_mod.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				String got = textField.getText();
				textField.setText("SQRT(" + got + ")");
			}
		});
		button_mod.setBounds(425, 135, 50, 40);
		panel.add(button_mod);
		
		JButton button_pow = new JButton("^");
		button_pow.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				String got = textField.getText();
				textField.setText(got+"^");
			}
		});
		button_pow.setBounds(485, 135, 50, 40);
		panel.add(button_pow);
		
		JButton button_div = new JButton(":");
		button_div.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				String got = textField.getText();
				textField.setText(got+":");
			}
		});
		button_div.setBounds(545, 135, 50, 40);
		panel.add(button_div);
		
		JButton button_res = new JButton("=");
		button_res.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				String got = textField.getText();
				double res_of_alg = Rpn.expr_to_result(got,mem);
				//
				textField.setText(Double.toString(res_of_alg));
			}
		});
		button_res.setBounds(305, 185, 50, 40);
		panel.add(button_res);

		JButton button_7 = new JButton("7");
		button_7.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				String got = textField.getText();
				textField.setText(got+"7");
			}
		});
		button_7.setBounds(365, 185, 50, 40);
		panel.add(button_7);
		
		JButton button_8 = new JButton("8");
		button_8.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				String got = textField.getText();
				textField.setText(got+"8");
			}
		});
		button_8.setBounds(425, 185, 50, 40);
		panel.add(button_8);
		
		JButton button_9 = new JButton("9");
		button_9.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				String got = textField.getText();
				textField.setText(got+"9");
			}
		});
		button_9.setBounds(485, 185, 50, 40);
		panel.add(button_9);
		
		JButton button_mul = new JButton("x");
		button_mul.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				String got = textField.getText();
				textField.setText(got+"*");
			}
		});
		button_mul.setBounds(545, 185, 50, 40);
		panel.add(button_mul);
		
		JButton button_dot = new JButton(".");
		button_dot.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				String got = textField.getText();
				textField.setText(got+".");
			}
		});
		button_dot.setBounds(305, 235, 50, 40);
		panel.add(button_dot);

		JButton button_4 = new JButton("4");
		button_4.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				String got = textField.getText();
				textField.setText(got+"4");
			}
		});
		button_4.setBounds(365, 235, 50, 40);
		panel.add(button_4);
		
		JButton button_5 = new JButton("5");
		button_5.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				String got = textField.getText();
				textField.setText(got+"5");
			}
		});
		button_5.setBounds(425, 235, 50, 40);
		panel.add(button_5);
		
		JButton button_6 = new JButton("6");
		button_6.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				String got = textField.getText();
				textField.setText(got+"6");
			}
		});
		button_6.setBounds(485, 235, 50, 40);
		panel.add(button_6);
		
		JButton button_sub = new JButton("-");
		button_sub.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				String got = textField.getText();
				textField.setText(got+"-");
			}
		});
		button_sub.setBounds(545, 235, 50, 40);
		panel.add(button_sub);
				
		JButton button_0 = new JButton("0");
		button_0.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				String got = textField.getText();
				textField.setText(got+"0");
			}
		});
		button_0.setBounds(305, 285, 50, 40);
		panel.add(button_0);

		JButton button_1 = new JButton("1");
		button_1.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				String got = textField.getText();
				textField.setText(got+"1");
			}
		});
		button_1.setBounds(365, 285, 50, 40);
		panel.add(button_1);
		
		JButton button_2 = new JButton("2");
		button_2.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				String got = textField.getText();
				textField.setText(got+"2");
			}
		});
		button_2.setBounds(425, 285, 50, 40);
		panel.add(button_2);
		
		JButton button_3 = new JButton("3");
		button_3.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				String got = textField.getText();
				textField.setText(got+"3");
			}
		});
		button_3.setBounds(485, 285, 50, 40);
		panel.add(button_3);
		
		JButton button_add = new JButton("+");
		button_add.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				String got = textField.getText();
				textField.setText(got+"+");
			}
		});
		button_add.setBounds(545, 285, 50, 40);
		panel.add(button_add);
				
		
		
		
		
		// ----------------
		
		JButton btnSaveMem = new JButton("Save Mem");
		btnSaveMem.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				JFileChooser fileChooser = new JFileChooser();
				if (fileChooser.showSaveDialog(frame) == JFileChooser.APPROVE_OPTION) {
				  File file = fileChooser.getSelectedFile();
				  
				  try (ObjectOutputStream output =
							new ObjectOutputStream(new FileOutputStream(file.getPath()))) {
					  			output.writeObject(mem);
						} catch (IOException i) {
							i.printStackTrace();
							return;
						}
				}
				
			}
		});
		
		btnSaveMem.setBounds(305, 335, 140, 25);
		btnSaveMem.setFont(font_of_buttons);
		panel.add(btnSaveMem);
		
		JButton btnLoadMem = new JButton("Load Mem");
		btnLoadMem.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				
				
				JFileChooser fileChooser = new JFileChooser();
				if (fileChooser.showOpenDialog(frame) == JFileChooser.APPROVE_OPTION) {
				  File file = fileChooser.getSelectedFile();

				  try (ObjectInputStream input =
							new ObjectInputStream(new FileInputStream(file.getPath()))) {
							mem = (Memory) input.readObject();

							
							
							
						} catch (IOException i) {
							
						} catch (ClassNotFoundException c) {
							
						}
				  
				  FuncListModel.removeAllElements();
				  int mem_func_ammount = mem.funcs.size();
				  for(Func object: mem.funcs)
					  FuncListModel.addElement(object.Name + "(" + object.Arity + " args) := " + object.Body);

				  VarListModel.removeAllElements();
				  int mem_var_ammount = mem.vars.size();
				  for(Var object: mem.vars)
					  VarListModel.addElement(object.Letter + " := " + object.Meaning);
					  
				  
				  
				}
				
				
			}
		});
		
		btnLoadMem.setBounds(305, 375, 140, 25);
		btnLoadMem.setFont(font_of_buttons);
		panel.add(btnLoadMem);
		
		
		
		
		
		
	}
}
