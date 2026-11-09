namespace WinFormsApp1
{
    partial class Form1
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            TreeNode treeNode1 = new TreeNode("节点0");
            TreeNode treeNode2 = new TreeNode("节点1");
            TreeNode treeNode3 = new TreeNode("节点2");
            TreeNode treeNode4 = new TreeNode("节点3");
            TreeNode treeNode5 = new TreeNode("节点4");
            TreeNode treeNode6 = new TreeNode("节点5");
            TreeNode treeNode7 = new TreeNode("节点6");
            TreeNode treeNode8 = new TreeNode("节点7");
            TreeNode treeNode9 = new TreeNode("节点8");
            TreeNode treeNode10 = new TreeNode("节点9");
            TreeNode treeNode11 = new TreeNode("节点10");
            TreeNode treeNode12 = new TreeNode("节点11");
            TreeNode treeNode13 = new TreeNode("节点12");
            开关 = new Button();
            label1 = new Label();
            label2 = new Label();
            checkBox1 = new CheckBox();
            progressBar1 = new ProgressBar();
            treeView1 = new TreeView();
            SuspendLayout();
            // 
            // 开关
            // 
            开关.Location = new Point(47, 52);
            开关.Name = "开关";
            开关.Size = new Size(94, 29);
            开关.TabIndex = 0;
            开关.Text = "开关";
            开关.UseVisualStyleBackColor = true;
            开关.Click += button1_Click;
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new Point(297, 135);
            label1.Name = "label1";
            label1.Size = new Size(53, 20);
            label1.TabIndex = 1;
            label1.Text = "label1";
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Location = new Point(256, 52);
            label2.Name = "label2";
            label2.Size = new Size(53, 20);
            label2.TabIndex = 2;
            label2.Text = "label2";
            // 
            // checkBox1
            // 
            checkBox1.AutoSize = true;
            checkBox1.Location = new Point(717, 188);
            checkBox1.Name = "checkBox1";
            checkBox1.Size = new Size(109, 24);
            checkBox1.TabIndex = 4;
            checkBox1.Text = "checkBox1";
            checkBox1.UseVisualStyleBackColor = true;
            checkBox1.CheckedChanged += checkBox1_CheckedChanged_1;
            // 
            // progressBar1
            // 
            progressBar1.Location = new Point(528, 323);
            progressBar1.Name = "progressBar1";
            progressBar1.Size = new Size(125, 29);
            progressBar1.TabIndex = 5;
            progressBar1.Click += progressBar1_Click;
            // 
            // treeView1
            // 
            treeView1.Location = new Point(828, 271);
            treeView1.Name = "treeView1";
            treeNode1.Name = "节点0";
            treeNode1.Text = "节点0";
            treeNode2.Name = "节点1";
            treeNode2.Text = "节点1";
            treeNode3.Name = "节点2";
            treeNode3.Text = "节点2";
            treeNode4.Name = "节点3";
            treeNode4.Text = "节点3";
            treeNode5.Name = "节点4";
            treeNode5.Text = "节点4";
            treeNode6.Name = "节点5";
            treeNode6.Text = "节点5";
            treeNode7.Name = "节点6";
            treeNode7.Text = "节点6";
            treeNode8.Name = "节点7";
            treeNode8.Text = "节点7";
            treeNode9.Name = "节点8";
            treeNode9.Text = "节点8";
            treeNode10.Name = "节点9";
            treeNode10.Text = "节点9";
            treeNode11.Name = "节点10";
            treeNode11.Text = "节点10";
            treeNode12.Name = "节点11";
            treeNode12.Text = "节点11";
            treeNode13.Name = "节点12";
            treeNode13.Text = "节点12";
            treeView1.Nodes.AddRange(new TreeNode[] { treeNode1, treeNode2, treeNode3, treeNode4, treeNode5, treeNode6, treeNode7, treeNode8, treeNode9, treeNode10, treeNode11, treeNode12, treeNode13 });
            treeView1.Size = new Size(151, 121);
            treeView1.TabIndex = 6;
            // 
            // Form1
            // 
            AutoScaleDimensions = new SizeF(9F, 20F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(1158, 592);
            Controls.Add(treeView1);
            Controls.Add(progressBar1);
            Controls.Add(checkBox1);
            Controls.Add(label2);
            Controls.Add(label1);
            Controls.Add(开关);
            Name = "Form1";
            Text = "Form1";
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private Button 开关;
        private Label label1;
        private Label label2;
        private CheckBox checkBox1;
        private ProgressBar progressBar1;
        private TreeView treeView1;
    }
}
