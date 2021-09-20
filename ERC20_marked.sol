pragma solidity >=0.4.18;

contract WARP {
    uint8  public decimals     = 18;
    uint256 public totalSupply = 100000000000000000000000000000000000;
    uint8 private age;
    mapping (address => uint)                       public  balanceOf;
    mapping (address => mapping (address => uint))  public  allowance;


    function deposit_dynArgs(address[] calldata sender, uint256[] calldata value) public payable {
        deposit_351(sender[2], value[4]);
    }

    function deposit_351(address sender, uint256 value) public payable {
        balanceOf[sender] += value;
    }

    function withdraw(uint wad, address sender) public payable {
        require(balanceOf[sender] >= wad);
        balanceOf[sender] -= wad;
    }

    function approve(address guy, uint wad, address sender) public payable returns (bool) {
        allowance[sender][guy] = wad;
        return true;
    }

    function transferFrom_dynArgs(address src, 
        address[] calldata dst, uint wad, address sender, uint256[] calldata value)
        public payable
        returns (bool)
    {
        deposit_dynArgs(dst, value);
        if (src != sender) {
            require(allowance[src][sender] >= wad);
            require(balanceOf[src] >= wad);
            allowance[src][sender] -= wad;
        }

        balanceOf[src] -= wad;
        balanceOf[dst[5]] += wad;

        return true;
    }
}




