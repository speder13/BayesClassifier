   M S B A M L   `   `   `  ����F  BTest2Kinect, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null)clr-namespace:Test2KinectTest2Kinect  5   	       Test2Kinect.MainWindow   5   	   R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35\ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35D 9http://schemas.microsoft.com/winfx/2006/xaml/presentation    5   	   R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     5   	   @d2http://schemas.microsoft.com/expression/blend/2008     5   	   Jmc;http://schemas.openxmlformats.org/markup-compatibility/2006     5   	   +localclr-namespace:Test2Kinect     5   	   -   5          � Title$  KinectWindow��5   	   $	��900��6   $
��1400��6+    � 	Resources 5	         MediumGreyBrush �� Color  KinectPurpleBrush  KinectBlueBrush �� 
TargetType �� Property �� Value��%�   &	        &	 #     &	 F     '�� i     �� 5
   
   $ 	#ff6e6e6e��62   �� 5   
   $ 	#ff52318f��64   �� 5   
   $ 	#ff00BCF2��62   �� 5   
   " ��6   .�5      ��  �@J�SnapsToDevicePixels .  64   5      5(      .��5*      � -   5        $��
layoutGrid��5*      $��	10 0 10 0q�6    � RowDefinitions 5+   
   �� 5,      $
��Auto �6   �� 5-      $��* �6   �� 5.      $
��Auto �6   �� 5/      $
��Auto �6   .+�56   
   � $��1��6    � ColumnDefinitions 57      �� 58      $	��680 �6#   �� 59      $	��680 �6#   .+�5;      (� $��0��6    (� Stretch$
 None=�6&   $
��Left=�65   .��5<      �� -   5        $��Image��5<      $	��640��6%   $	��480��61   5=      (� 5>      $��1��6   $
 None=�6&   $��Right=�65   .��5?      �� -   5        $��Image2��5?      $	��400��6&   $	��200��62   5@      5A      �� 5D   
   -   5        $��	statusBar��5D   ?   $��3��6   $��Stretch=�6!   $��Bottom=�6P   
�������#��[  6~   .@�5E      �� $��0 0 0 10q�6   .��5F      �� -   5        $��statusBarText��5F      $��-1 0 0 0q�61   Kæft vi er seje!6U   5G      5H      5I      5J      