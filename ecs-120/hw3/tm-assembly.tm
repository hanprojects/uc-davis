states = {s0, s1, load, move, result, accept, reject,x, y, z, z1, next, add0, add1, Mx, My, Mxx, Myy}
input_alphabet = {0,1,^,L,:,M,x,y,A}
tape_alphabet_extra = {_}
start_state = s0
accept_state = accept
reject_state = reject
num_tapes = 4

delta =
    s0,^___->s1,^__^,RSSS;
    s1,L__^->load,L__^,RSSR;
    
    load,0___->load,0__0,RSSR;
    load,1___->load,1__1,RSSR;
    load,:___->z,:___,SSSL;
    load,0__1->load,0__0,RSSR;   
    load,1__0->load,1__1,RSSR;
    load,0__0->load,0__0,RSSR;   
    load,1__1->load,1__1,RSSR;
    load,0_^1->load,0_^0,RSSR;  
    load,1_^0->load,1_^1,RSSR;
    load,0_^0->load,0_^0,RSSR;
    load,1_^1->load,1_^1,RSSR;
    load,:_^_->z,:_^_,SSSL;
    load,0^_1->load,0^_0,RSSR;  
    load,1^_0->load,1^_1,RSSR;
    load,0^_0->load,0^_0,RSSR; 
    load,1^_1->load,1^_1,RSSR;
    load,:^__->z,:^__,SSSL;
    load,0^^1->load,0^^0,RSSR;  
    load,1^^0->load,1^^1,RSSR;
    load,0^^0->load,0^^0,RSSR;
    load,1^^1->load,1^^1,RSSR;
    load,:^^_->z,:^^_,RSSL;
    
    z,:__0->z,:__0,SSSL;
    z,:__1->z,:__1,SSSL;
    z,:^^0->z,:^^0,SSSL;
    z,:^^1->z,:^^1,SSSL;
    z,_^^0->next,_^^0,SSSL;
    z,_^^1->next,_^^1,SSSL;
    z,L^^0->next,L^^0,SSSL;
    z,L^^1->next,L^^1,SSSL;
    z,A^^0->z1,A^^0,SRRS;
    z,A^^1->z1,A^^1,SRRS;
    z,:__^->next,:__^,RSSS;
    z,M^^0->z,M^^0,SSSL;
    z,M^^1->z,M^^1,SSSL;
    z,M^^^->next,M^^^,SSSS;
    z,:_^0->z,:_^0,SSSL;
    z,:_^1->z,:_^1,SSSL;
    z,:_^^->next,:_^^,RSSS;
    z,:^_0->z,:^_0,SSSL;
    z,:^_1->z,:^_1,SSSL;
    z,:^_^->next,:^_^,RSSS;
    
    next,_^^0->next,_^^0,SSSL;
    next,_^^1->next,_^^1,SSSL;
    next,L^^0->next,L^^0,SSSL;
    next,L^^1->next,L^^1,SSSL;
    next,___^->result,___^,SSSR;
    next,_^^^->result,_^^^,SSSR;
    next,__^^->result,__^^,SSSR;
    next,_^_^->result,_^_^,SSSR;
    next,M__^->move,M__^,RSSS;
    next,L__^->load,L__^,RSSR;
    next,L_^^->load,L_^^,RSSR;
    next,L^_^->load,L^_^,RSSR;
    next,A__^->z1,A__^,SSSR;
    next,A^_^->z1,A^_^,SRSR;
    next,A^^^->z1,A^^^,SRRR;
    next,A_^^->z1,A_^^,SSRR;
    next,:^^^->next,:^^^,RSSS;
    next,L^^^->load,L^^^,RSSR;
    next,M^^^->move,M^^^,RSSS;
    next,M_^^->move,M_^^,RSSS;
    next,M^_^->move,M^_^,RSSS;
    
    result,___0->accept,___0,SSSS;
    result,___1->accept,___1,SSSS;
    result,_^^0->accept,_^^0,SSSS;
    result,_^^1->accept,_^^1,SSSS;
    result,__^0->accept,__^0,SSSS;
    result,_^_0->accept,_^_0,SSSS;
    result,_^_1->accept,_^_1,SSSS;
    result,__^1->accept,__^1,SSSS;
    
    move,x__^->Mx,x^_^,SSSS;
    move,y__^->My,y_^^,SSSS;
    move,y_^^->My,y_^^,SSSS;
    move,x^^^->Mx,x^^^,SSSS;
    move,y^^^->My,y^^^,SSSS;
    move,y^_^->My,y^_^,SSSS;
    move,x_^^->Mx,x_^^,SSSS;
    move,M_^^->move,M_^^,RSSS;
    move,M^_^->move,M^_^,RSSS;
    move,x^_^->Mxx,x^_^,SRSR;
    
    Mx,x^_^->Mxx,x^_^,SRSR;
    Mx,x^^^->Mxx,x^^^,SRSR;
    Mx,x_^^->x,x_^^,SLSS;
    Mx,x__^->x,x__^,SLSS;
    
    Mxx,x__0->Mxx,x0_0,SRSR;
    Mxx,x__1->Mxx,x1_1,SRSR;
    Mxx,x0^0->Mxx,x0^0,SRSR;
    Mxx,x1^1->Mxx,x1^1,SRSR;
    Mxx,x1^0->Mxx,x0^0,SRSR;
    Mxx,x0^1->Mxx,x1^1,SRSR;
    Mxx,x___->z,x___,RSSL;
    Mxx,x_^_->z,x_^_,RSSL;
    Mxx,x0_0->Mxx,x0_0,SRSR;
    Mxx,x1_0->Mxx,x0_0,SRSR;
    Mxx,x1_1->Mxx,x1_1,SRSR;
    Mxx,x0_1->Mxx,x1_1,SRSR;
    
    My,y_^^->Myy,y_^^,SSRR;
    My,y^^^->Myy,y^^^,SSRR;
    My,y^_^->y,y^_^,SSLS;
    Myy,y_00->Myy,y_00,SSRR;
    Myy,y_10->Myy,y_00,SSRR;
    Myy,y_11->Myy,y_11,SSRR;
    Myy,y_01->Myy,y_11,SSRR;
    
    Myy,y__0->Myy,y_00,SSRR;
    Myy,y__1->Myy,y_11,SSRR;
    Myy,y___->z,y___,RSSL;
    Myy,y^00->Myy,y^00,SSRR;
    Myy,y^11->Myy,y^11,SSRR;
    Myy,y^10->Myy,y^00,SSRR;
    Myy,y^01->Myy,y^11,SSRR;
    Myy,y^__->z,y^__,RSSL;
    
    z1,A__0->z1,A__0,SSSR;
    z1,A11_->z1,A11_,SRRS;
    z1,A00_->z1,A00_,SRRS;
    z1,A01_->z1,A01_,SRRS;
    z1,A10_->z1,A10_,SRRS;
    z1,A__1->z1,A__1,SSSR;
    z1,A^_^->z1,A^_^,SRSR;
    z1,A0_0->z1,A0_0,SRSR;
    z1,A1_1->z1,A1_1,SRSR;
    z1,A1_0->z1,A1_0,SRSR;
    z1,A0_1->z1,A0_1,SRSR;
    z1,A^^^->z1,A^^^,SRRR;
    z1,A000->z1,A000,SRRR;
    z1,A111->z1,A111,SRRR;
    z1,A010->z1,A010,SRRR;
    z1,A011->z1,A011,SRRR;
    z1,A001->z1,A001,SRRR;
    z1,A110->z1,A110,SRRR;
    z1,A100->z1,A100,SRRR;
    z1,A101->z1,A101,SRRR;
    z1,A_^^->z1,A_^^,SSRR;
    z1,A_00->z1,A_00,SSRR;
    z1,A_11->z1,A_11,SSRR;
    z1,A_10->z1,A_10,SSRR;
    z1,A_01->z1,A_01,SSRR;
    z1,A___->add0,A___,SLLL;
    
    add0,A001->add0,A000,SLLL;
    add0,A000->add0,A000,SLLL;
    add0,A100->add0,A101,SLLL;
    add0,A101->add0,A101,SLLL;
    add0,A010->add0,A011,SLLL;
    add0,A011->add0,A011,SLLL;
    add0,A110->add1,A110,SLLL;
    add0,A111->add1,A110,SLLL;
    add0,A^^^->next,A^^^,RSSS;
    
    add1,A000->add0,A001,SLLL;
    add1,A001->add0,A001,SLLL;
    add1,A100->add1,A100,SLLL;
    add1,A101->add1,A100,SLLL;
    add1,A111->add1,A111,SLLL;
    add1,A110->add1,A111,SLLL;
    add1,A010->add1,A010,SLLL;
    add1,A011->add1,A010,SLLL;
    add1,A^^^->next,A^^^,RSSS;
    
    x,x0^^->x,x0^^,SLSS;
    x,x1^^->x,x1^^,SLSS;
    x,x^^^->Mxx,x^^^,SRSR;
    x,x0_^->x,x0_^,SLSS;
    x,x1_^->x,x1_^,SLSS;
    x,x^_^->Mxx,x^_^,SRSR;
    
    y,y^0^->y,y^0^,SSLS;
    y,y^1^->y,y^1^,SSLS;
    y,y^^^->Myy,y^^^,SSRR;
    y,y_0^->y,y_0^,SSLS;